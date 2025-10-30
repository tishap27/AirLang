/*
************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Summer, 2025
* Author: Tisha Patel
************************************************************
#
# ECHO "=---------------------------------------="
# ECHO "|  COMPILERS - ALGONQUIN COLLEGE (S25)  |"
# ECHO "=---------------------------------------="
# ECHO "    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@    "
# ECHO "    @@                             @@    "
# ECHO "    @@            __|__            @@    "
# ECHO "    @@-------@--o--(_)--o--@-------@@    "
# ECHO "    @@                             @@    "
# ECHO "    @@        A I R L A N G        @@    "
# ECHO "    @@                             @@    "
# ECHO "    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@    "
# ECHO "                                         "
# ECHO "[MetarFetcher SCRIPT ...................]"
# ECHO "                                         "
*/

/*
************************************************************
* File name: MetarFetcher.c
* Compiler: MS Visual Studio 2022
* Course: CST 8152 – Compilers, Lab Section: [011, 012]
* Date: Oct 28, 2025
* Professor: Paulo Sousa
* Purpose: This file implements the METAR data fetching module
* including API interaction using libcurl and JSON parsing.
************************************************************
*/



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <curl/curl.h>
#include "Step5Writer.h"

// Callback for curl
struct MemoryStruct {
    char* memory;
    size_t size;
};

static size_t write_metar_callback(void* contents, size_t size, size_t nmemb, void* userp) {
    size_t realsize = size * nmemb;
    struct MemoryStruct* mem = (struct MemoryStruct*)userp;

    char* ptr = realloc(mem->memory, mem->size + realsize + 1);
    if (!ptr) {
        printf("ERROR: Not enough memory for METAR response\n");
        return 0;
    }

    mem->memory = ptr;
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}

airlang_intg fetch_metar_from_api(const char* icao_code) {
    CURL* curl;
    CURLcode res;
    struct MemoryStruct chunk;
    char url[512];

    chunk.memory = malloc(1);
    chunk.size = 0;

    // Validate ICAO
    if (strlen(icao_code) != 4) {
        printf("ERROR: Invalid ICAO code '%s'\n", icao_code);
        free(chunk.memory);
        return 0;
    }

    // Convert to uppercase
    char icao_upper[5];
    strncpy_s(icao_upper, sizeof(icao_upper), icao_code, 4);
    icao_upper[4] = '\0';
    for (int i = 0; i < 4; i++) {
        icao_upper[i] = toupper(icao_upper[i]);
    }

    // Build API URL
    snprintf(url, sizeof(url),
        "https://aviationweather.gov/api/data/metar?ids=%s&format=json&taf=false",
        icao_upper);

    printf("Fetching METAR for %s...\n", icao_upper);

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_metar_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&chunk);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L);

        // First attempt: Try with original User-Agent
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "AirLang-DSL/1.0");
        res = curl_easy_perform(curl);

        if (res != CURLE_OK || chunk.size == 0 || strstr(chunk.memory, "403 ERROR")) {
            //DEBUG
            //printf("First attempt blocked, retrying with browser User-Agent...\n");
           
            //reset memory
            free(chunk.memory);
            chunk.memory = malloc(1);
            chunk.size = 0;


            // Retry with Chrome User-Agent
            curl_easy_setopt(curl, CURLOPT_USERAGENT,
                "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/130.0.0.0 Safari/537.36");
            res = curl_easy_perform(curl);
        }

        if (res != CURLE_OK) {
            printf("ERROR: Failed to fetch METAR: %s\n", curl_easy_strerror(res));
            curl_easy_cleanup(curl);
            curl_global_cleanup();
            free(chunk.memory);
            return 0;
        }

        if (chunk.memory != NULL) {
            //DEBUG
            //printf("Full JSON:\n%s\n", chunk.memory);

            // Extract raw METAR from JSON
            char* metar_pos = strstr(chunk.memory, "\"rawOb\":");
            if (metar_pos) {
                char* start = strstr(metar_pos, ":");

                if (start) {
                    start = strchr(start, '"') + 1; // move past the opening "
                    char* end = strchr(start, '"');
                    if (end && end > start) {
                        int len = (int)(end - start);
                        if (len > 0 && len < 512) {
                            char metar_string[512] = { 0 };
                            strncpy_s(metar_string, sizeof(metar_string), start, len);
                            metar_string[len] = '\0';

                            // Store METAR
                            char var_name[64];
                            snprintf(var_name, sizeof(var_name), "METAR_%s", icao_upper);
                            assign_string_variable(var_name, metar_string);

                            printf("METAR received for %s\n", icao_upper);
                            printf("Raw METAR: %s\n", metar_string);

                            // Parse it
                            parseMetar(metar_string, icao_upper);

                            curl_easy_cleanup(curl);
                            curl_global_cleanup();
                            free(chunk.memory);
                            return 1;
                        }
                    }
                }
            }

            curl_easy_cleanup(curl);
            free(chunk.memory);
        }
    }

    curl_global_cleanup();
    return 0;
}