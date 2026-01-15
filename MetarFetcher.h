/*
************************************************************
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
* File name: MetarFetcher.h
* Compiler: MS Visual Studio 2022
* Date: Oct 28, 2025
* Professor: Paulo Sousa
* Purpose: Header file declaring the METAR fetching functions
************************************************************
*/
#ifndef METAR_FETCHER_H
#define METAR_FETCHER_H

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

// Forward declarations - no curl headers here!
airlang_intg fetch_metar_from_api(const char* icao_code);
airlang_void fetchMetarData(const airlang_strg station_id);

#endif
