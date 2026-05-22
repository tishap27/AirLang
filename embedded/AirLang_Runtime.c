
 /*
 ************************************************************
 * Author: Tisha Patel
 * AirLang_Runtime.c 
 ************************************************************
 #
 # ECHO "=---------------------------------------="
 # ECHO "|             COMPILERS                 |"
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
 # ECHO "[Runtime SCRIPT .........................]"
 # ECHO "                                         "
 */


#define _CRT_SECURE_NO_WARNINGS
#include "AirLang.h"
#include <stdlib.h>
#include <stdio.h>  
#include <string.h>
#include <ctype.h>

#ifdef _WIN32
#  define rt_strtok(s, d, c) strtok_s(s, d, c)
#else
#  define rt_strtok(s, d, c) strtok_r(s, d, c)
#endif

static int  rt_find(AIR_Runtime* rt, const char* name);
static void rt_set_num(AIR_Runtime* rt, const char* name, double val);
static void rt_set_str(AIR_Runtime* rt, const char* name, const char* val);
static void rt_extract_airport_from_assignment(AIR_Runtime* rt, const char* name, char* out);

 //internal variable
typedef enum { RT_NUM = 0, RT_STR = 1 } RT_VarType;

typedef struct {
    char       name[64];
    RT_VarType type;
    double     num;
    char       str[256];
} RT_Var;

// runtime context 
struct AIR_Runtime {
    AIR_OutputCallback cb;
    void* cb_data;
    int                verbose;
    RT_Var             vars[AIR_MAX_VARS];
    int                var_count;
    char               current_airport[16];  
    int                if_result;            
    int                in_if;                
    int                skip;                 
};


AIR_Runtime* AIR_Init(void) {
    return (AIR_Runtime*)calloc(1, sizeof(AIR_Runtime));
}

void AIR_Free(AIR_Runtime* rt) { if (rt) free(rt); }

AIR_Status AIR_Reset(AIR_Runtime* rt) {
    if (!rt) return AIR_ERR_NULL;
    AIR_OutputCallback cb = rt->cb; void* d = rt->cb_data; int v = rt->verbose;
    memset(rt, 0, sizeof(*rt));
    rt->cb = cb; rt->cb_data = d; rt->verbose = v;
    return AIR_OK;
}

void AIR_SetOutputCallback(AIR_Runtime* rt, AIR_OutputCallback cb, void* data) {
    if (rt) { rt->cb = cb; rt->cb_data = data; }
}

void AIR_SetVerbose(AIR_Runtime* rt, int v) { if (rt) rt->verbose = v; }

const char* AIR_Version(void) { return AIRLANG_VERSION_STR; }

//not all are implemented yet
// 


// INTERPRETER CORE  like Step5Writer.c calculate()


static void rt_emit(AIR_Runtime* rt, const char* msg) {
    if (rt->cb) rt->cb(msg, rt->cb_data);
    else        printf("%s\n", msg);
}

static void rt_trim(char* s) {
    char* p = s; int l;
    if (!s || !*s) return;
    while (isspace((unsigned char)*p)) p++;
    l = (int)strlen(p);
    while (l > 0 && isspace((unsigned char)p[l - 1])) l--;
    memmove(s, p, l);
    s[l] = '\0';
}

static double rt_eval_expr(AIR_Runtime* rt, const char* expr) {
    char clean[256] = { 0 }; int i = 0, j = 0;
    char ops[10] = { 0 }; int op_count = 0;
    double vals[10] = { 0 }; int val_count = 0;
    char tmp[256]; char* tok, * ctx2;

    /* remove spaces */
    while (expr[i]) { if (!isspace((unsigned char)expr[i])) clean[j++] = expr[i]; i++; }
    clean[j] = '\0';

    /* AIRPATH keyword */
    if (strcmp(clean, "AIRPATH") == 0) {
        int c1 = -1, c2 = -1, k;
        for (k = 0;k < rt->var_count;k++) {
            if (rt->vars[k].type == RT_STR &&
                AIR_IsCoordFormat(rt->vars[k].str)) {
                c1 = c2; c2 = k;
            }
        }
        if (c1 != -1 && c2 != -1) {
            double la1, lo1, la2, lo2;
            char t1[128], t2[128]; char* cm; 
            strncpy(t1, rt->vars[c1].str, 127);
            strncpy(t2, rt->vars[c2].str, 127);
            cm = strchr(t1, ','); if (cm) { *cm = '\0'; la1 = atof(t1); lo1 = atof(cm + 1); }
            else return 0;
            cm = strchr(t2, ','); if (cm) { *cm = '\0'; la2 = atof(t2); lo2 = atof(cm + 1); }
            else return 0;
            return AIR_GreatCircleNM(la1, lo1, la2, lo2);
        }
        return 0;
    }

    /* HEADWIND() */
    if (strcmp(clean, "HEADWIND()") == 0) {
        char wd[32], ws[32], rw[32]; int wi, si, ri;
        snprintf(wd, 32, "%s_WIND_DIR", rt->current_airport);
        snprintf(ws, 32, "%s_WIND_SPEED", rt->current_airport);
        snprintf(rw, 32, "%s_RUNWAY", rt->current_airport);
        wi = rt_find(rt, wd); si = rt_find(rt, ws); ri = rt_find(rt, rw);
        if (wi != -1 && si != -1 && ri != -1) {
            AIR_WindComponents wc = AIR_CalcWind(
                rt->vars[wi].num, rt->vars[si].num, rt->vars[ri].num);
            return wc.headwind;
        }
        return 0;
    }

    /* CROSSWIND() */
    if (strcmp(clean, "CROSSWIND()") == 0) {
        char wd[32], ws[32], rw[32]; int wi, si, ri;
        snprintf(wd, 32, "%s_WIND_DIR", rt->current_airport);
        snprintf(ws, 32, "%s_WIND_SPEED", rt->current_airport);
        snprintf(rw, 32, "%s_RUNWAY", rt->current_airport);
        wi = rt_find(rt, wd); si = rt_find(rt, ws); ri = rt_find(rt, rw);
        if (wi != -1 && si != -1 && ri != -1) {
            AIR_WindComponents wc = AIR_CalcWind(
                rt->vars[wi].num, rt->vars[si].num, rt->vars[ri].num);
            return wc.crosswind;
        }
        return 0;
    }

    /* extract operators */
    for (i = 0;clean[i];i++)
        if (clean[i] == '+' || clean[i] == '-' ||
            clean[i] == '*' || clean[i] == '/') ops[op_count++] = clean[i];

    /* tokenize by operators */
    strncpy(tmp, clean, 255);
    tok = rt_strtok(tmp, "+-*/", &ctx2);
    while (tok && val_count < 10) {
        if (isdigit((unsigned char)tok[0]) ||
            (tok[0] == '-' && isdigit((unsigned char)tok[1]))) {
            vals[val_count++] = atof(tok);
        }
        else {
            int vi = rt_find(rt, tok);
            vals[val_count++] = (vi != -1 && rt->vars[vi].type == RT_NUM)
                ? rt->vars[vi].num : 0.0;
        }
        tok = rt_strtok(NULL, "+-*/", &ctx2);
    }

    /* evaluate with precedence */
    if (val_count == 0) return 0;
    for (i = 0;i < op_count;i++) {
        if (ops[i] == '*') { vals[i + 1] = vals[i] * vals[i + 1]; vals[i] = 0; ops[i] = '+'; }
        if (ops[i] == '/') { if (vals[i + 1] != 0) vals[i + 1] = vals[i] / vals[i + 1]; vals[i] = 0; ops[i] = '+'; }
    }
    {
        double r = vals[0];
        for (i = 0;i < op_count;i++) {
            if (ops[i] == '+') r += vals[i + 1];
            if (ops[i] == '-') r -= vals[i + 1];
        }
        return r;
    }
}

static void rt_handle_print(AIR_Runtime* rt, const char* line) {
    char buf[512] = { 0 }; int bp = 0;
    const char* s = strchr(line, '{'), * e = strrchr(line, '}');
    if (!s || !e || s >= e) return;
    s++;
    while (*s && *s != '}' && bp < 511) {
        if (*s == '"') {
            s++;
            while (*s != '"' && *s && bp < 511) buf[bp++] = *s++;
            if (*s == '"') s++;
        }
        else if (isalpha((unsigned char)*s)) {
            char vn[64] = { 0 }; int vp = 0;
            while ((isalnum((unsigned char)*s) || *s == '_') && vp < 63) vn[vp++] = *s++;
            {
                int vi = rt_find(rt, vn);
                if (vi != -1) {
                    char nb[32];
                    const char* val = (rt->vars[vi].type == RT_NUM)
                        ? (snprintf(nb, 32,
                            (rt->vars[vi].num == (int)rt->vars[vi].num)
                            ? "%.0f" : "%.2f",
                            rt->vars[vi].num), nb)
                        : rt->vars[vi].str;
                    while (*val && bp < 511) buf[bp++] = *val++;
                }
            }
        }
        else if (*s == '+') { s++; while (*s && isspace((unsigned char)*s))s++; }
        else if (isspace((unsigned char)*s)) { buf[bp++] = *s++; }
        else s++;
    }
    buf[bp] = '\0';
    rt_emit(rt, buf);
}




static int rt_eval_condition(AIR_Runtime* rt, const char* cond) {
    char tmp[128] = { 0 }; int i = 0, j = 0; char* op;
    while (cond[i]) { if (!isspace((unsigned char)cond[i]))tmp[j++] = cond[i];i++; }
    tmp[j] = '\0';
    if ((op = strstr(tmp, ">")) != NULL) {
        char l[64] = { 0 }, r[64] = { 0 };
        strncpy(l, tmp, (int)(op - tmp));
        strncpy(r, op + 1, 63);
        double lv, rv;
        {
            int vi = rt_find(rt, l);
            lv = isdigit((unsigned char)l[0]) ? atof(l)
                : (vi != -1 ? rt->vars[vi].num : 0.0);
        }
        {
            int vi = rt_find(rt, r);
            rv = isdigit((unsigned char)r[0]) ? atof(r)
                : (vi != -1 ? rt->vars[vi].num : 0.0);
        }
        return lv > rv;
    }
    return 0;
}

static void rt_inject_metar(AIR_Runtime* rt, const AIR_METARResult* mr) {
    char vn[64];
    snprintf(vn, 64, "%s_WIND_DIR", mr->station); rt_set_num(rt, vn, mr->wind_dir);
    snprintf(vn, 64, "%s_WIND_SPEED", mr->station); rt_set_num(rt, vn, mr->wind_speed_kt);
    snprintf(vn, 64, "%s_WIND_GUST", mr->station); rt_set_num(rt, vn, mr->wind_gust_kt);
    snprintf(vn, 64, "%s_TEMP", mr->station); rt_set_num(rt, vn, mr->temp_c);
    snprintf(vn, 64, "%s_DEWPOINT", mr->station); rt_set_num(rt, vn, mr->dewpoint_c);
    snprintf(vn, 64, "%s_VISIBILITY", mr->station); rt_set_num(rt, vn, mr->visibility);
    snprintf(vn, 64, "%s_ALTIMETER", mr->station); rt_set_num(rt, vn, mr->altimeter_inhg);
    snprintf(vn, 64, "METAR_%s", mr->station); rt_set_str(rt, vn, mr->raw);
    strncpy(rt->current_airport, mr->station, 15);
}

static void rt_extract_airport_from_assignment(AIR_Runtime* rt,
    const char* name,
    char* out) {
    (void)rt;
    char* u = strchr(name, '_');
    if (u) { int l = (int)(u - name); if (l > 0 && l < 16) { strncpy(out, name, l);out[l] = '\0'; } }
}

static void rt_calculate(AIR_Runtime* rt, char* line) {
    char vn[64] = { 0 };
    rt_trim(line);
    if (!line || !*line || line[0] == '^' || line[0] == '%') return;
    if (strstr(line, "^^")) return;

    /* IF / ELSE / ENDIF */
    if (strncmp(line, "IF ", 3) == 0) {
        char* then = strstr(line, " THEN");
        if (then) {
            char cond[128] = { 0 }; int cl = (int)(then - (line + 3));
            strncpy(cond, line + 3, cl); rt_trim(cond);
            rt->if_result = rt_eval_condition(rt, cond);
            rt->in_if = 1; rt->skip = !rt->if_result;
        } return;
    }
    if (strncmp(line, "ELSE", 4) == 0) { rt->skip = rt->if_result; return; }
    if (strncmp(line, "ENDIF", 5) == 0) { rt->skip = 0;rt->if_result = 0;rt->in_if = 0; return; }
    if (rt->skip) return;

    /* PRINT */
    if (strstr(line, "PRINT") && strchr(line, '{')) { rt_handle_print(rt, line); return; }

    /* REQUEST METAR FROM */
    if (strstr(line, "REQUEST") && strstr(line, "METAR") && strstr(line, "FROM")) {
        char* q1 = strchr(line, '"'), * q2; char icao[16] = { 0 };
        if (q1) {
            q2 = strchr(q1 + 1, '"');
            if (q2) {
                int l = (int)(q2 - q1 - 1);
                if (l == 4) {
                    strncpy(icao, q1 + 1, 4);
                    AIR_METARResult mr = AIR_ParseMETAR(icao);
                    if (mr.valid) rt_inject_metar(rt, &mr);
                }
            }
        } return;
    }

    /* METAR: "raw string" */
    if (strncmp(line, "METAR", 5) == 0 && strchr(line, ':')) {
        char* q1 = strchr(line, '"'), * q2;
        if (q1) {
            q2 = strchr(q1 + 1, '"');
            if (q2) {
                char raw[512] = { 0 }; int l = (int)(q2 - q1 - 1);
                strncpy(raw, q1 + 1, l < 511 ? l : 511);
                AIR_METARResult mr = AIR_ParseMETAR(raw);
                if (mr.valid) rt_inject_metar(rt, &mr);
            }
        } return;
    }

    /* colon assignment  Name: value; */
    if (strchr(line, ':') && !strstr(line, "PRINT")) {
        char* cp = strchr(line, ':');
        int nl = (int)(cp - line); int i = 0, j = 0;
        char val[256] = { 0 };
        for (;i < nl && j < 63;i++) if (!isspace((unsigned char)line[i])) vn[j++] = line[i];
        {
            char* vs = cp + 1; while (isspace((unsigned char)*vs))vs++;
            i = 0; while (vs[i] && vs[i] != ';' && i < 255) { val[i] = vs[i];i++; } val[i] = '\0';
            rt_trim(val);
            /* extract airport context from var name */
            rt_extract_airport_from_assignment(rt, vn, rt->current_airport);
            if (*val == '"') {
                char sv[256] = { 0 }; int si = 0; vs = val + 1;
                while (*vs && *vs != '"' && si < 255) sv[si++] = *vs++;
                rt_set_str(rt, vn, sv);
            }
            else if (AIR_IsCoordFormat(val)) {
                rt_set_str(rt, vn, val);
            }
            else {
                double nv = atof(val);
                if (nv != 0 || val[0] == '0') rt_set_num(rt, vn, nv);
                else rt_set_str(rt, vn, val);
            }
        } return;
    }

    /* equals assignment  Name = expr; */
    if (strchr(line, '=') && !strchr(line, ':')) {
        char* ep = strchr(line, '=');
        int nl = (int)(ep - line); int i = 0, j = 0;
        char expr[256] = { 0 };
        for (;i < nl && j < 63;i++) if (!isspace((unsigned char)line[i])) vn[j++] = line[i];
        {
            char* es = ep + 1; while (isspace((unsigned char)*es))es++;
            i = 0; while (es[i] && es[i] != ';' && i < 255) { expr[i] = es[i];i++; } expr[i] = '\0';
            rt_trim(expr);
            rt_extract_airport_from_assignment(rt, vn, rt->current_airport);
            rt_set_num(rt, vn, rt_eval_expr(rt, expr));
        } return;
    }
}

AIR_Status AIR_RunString(AIR_Runtime* rt, const char* source) {
    char* buf, * line, * ctx;
    if (!rt || !source) return AIR_ERR_NULL;
    buf = (char*)malloc(strlen(source) + 1);
    if (!buf) return AIR_ERR_ALLOC;
    strcpy(buf, source);
    line = rt_strtok(buf, "\n", &ctx);
    while (line) {
        rt_calculate(rt, line);
        line = rt_strtok(NULL, "\n", &ctx);
    }
    free(buf);
    return AIR_OK;
}
AIR_Status AIR_RunFile(AIR_Runtime* rt, const char* filepath) {
    FILE* f;
    char* buf;
    long  len;
    AIR_Status s;

    if (!rt || !filepath) return AIR_ERR_NULL;

    f = fopen(filepath, "r");
    if (!f) return AIR_ERR_FILE;

    fseek(f, 0, SEEK_END);
    len = ftell(f);
    rewind(f);

    buf = (char*)malloc(len + 1);
    if (!buf) { fclose(f); return AIR_ERR_ALLOC; }

    fread(buf, 1, len, f);
    buf[len] = '\0';
    fclose(f);

    s = AIR_RunString(rt, buf);
    free(buf);
    return s;
}

static int rt_find(AIR_Runtime* rt, const char* name) {
    int i;
    for (i = 0; i < rt->var_count; i++)
        if (strcmp(rt->vars[i].name, name) == 0) return i;
    return -1;
}

static void rt_set_num(AIR_Runtime* rt, const char* name, double val) {
    int i = rt_find(rt, name);
    if (i == -1) {
        if (rt->var_count >= AIR_MAX_VARS) return;
        i = rt->var_count++;
        strncpy(rt->vars[i].name, name, 63);
    }
    rt->vars[i].type = RT_NUM;
    rt->vars[i].num = val;
}
static void rt_set_str(AIR_Runtime* rt, const char* name, const char* val) {
    int i = rt_find(rt, name);
    if (i == -1) {
        if (rt->var_count >= AIR_MAX_VARS) return;
        i = rt->var_count++;
        strncpy(rt->vars[i].name, name, 63);
    }
    rt->vars[i].type = RT_STR;
    strncpy(rt->vars[i].str, val, 255);
}

AIR_Status AIR_GetNumber(AIR_Runtime* rt, const char* name, double* out) {
    int i;
    if (!rt || !name || !out) return AIR_ERR_NULL;
    i = rt_find(rt, name);
    if (i == -1) return AIR_ERR_NOT_FOUND;
    if (rt->vars[i].type != RT_NUM) return AIR_ERR_TYPE;
    *out = rt->vars[i].num;
    return AIR_OK;
}

AIR_Status AIR_GetString(AIR_Runtime* rt, const char* name, const char** out) {
    int i;
    if (!rt || !name || !out) return AIR_ERR_NULL;
    i = rt_find(rt, name);
    if (i == -1) return AIR_ERR_NOT_FOUND;
    if (rt->vars[i].type != RT_STR) return AIR_ERR_TYPE;
    *out = rt->vars[i].str;
    return AIR_OK;
}

AIR_Status AIR_GetVariable(AIR_Runtime* rt, const char* name, AIR_Variable* out) {
    int i;
    if (!rt || !name || !out) return AIR_ERR_NULL;
    i = rt_find(rt, name);
    if (i == -1) return AIR_ERR_NOT_FOUND;
    out->name = rt->vars[i].name;
    out->type = (rt->vars[i].type == RT_NUM) ? AIR_TYPE_NUMBER : AIR_TYPE_STRING;
    out->number = rt->vars[i].num;
    out->string = rt->vars[i].str;
    return AIR_OK;
}

AIR_Status AIR_GetAllVariables(AIR_Runtime* rt, AIR_Variable* vars,
    int max, int* count) {
    int i;
    if (!rt || !count) return AIR_ERR_NULL;
    *count = rt->var_count;
    if (!vars) return AIR_OK;
    for (i = 0; i < rt->var_count && i < max; i++) {
        vars[i].name = rt->vars[i].name;
        vars[i].type = (rt->vars[i].type == RT_NUM) ? AIR_TYPE_NUMBER : AIR_TYPE_STRING;
        vars[i].number = rt->vars[i].num;
        vars[i].string = rt->vars[i].str;
    }
    return AIR_OK;
}
//double          AIR_GreatCircleNM(double a, double b, double c, double d) { (void)a;(void)b;(void)c;(void)d; return 0.0; }
//AIR_WindComponents AIR_CalcWind(double a, double b, double c) { AIR_WindComponents r = { 0 };(void)a;(void)b;(void)c; return r; }
//AIR_METARResult AIR_ParseMETAR(const char* s) { AIR_METARResult r;memset(&r, 0, sizeof(r));r.wind_dir = -1;(void)s; return r; }
//AIR_METARResult AIR_FetchMETAR(AIR_Runtime* rt, const char* s) { AIR_METARResult r;memset(&r, 0, sizeof(r));r.wind_dir = -1;(void)rt;(void)s; return r; }
//int             AIR_ValidateICAO(const char* c) { (void)c; return 0; }
//int             AIR_IsAircraftIdentifier(const char* v) { (void)v; return 0; }
//int             AIR_IsCoordFormat(const char* v) { (void)v; return 0; }

const char* AIR_StatusString(AIR_Status s) {
    switch (s) {
    case AIR_OK:            return "OK";
    case AIR_ERR_ALLOC:     return "Memory allocation failure";
    case AIR_ERR_FILE:      return "File not found or unreadable";
    case AIR_ERR_SYNTAX:    return "Syntax error";
    case AIR_ERR_RUNTIME:   return "Runtime error";
    case AIR_ERR_NOT_FOUND: return "Variable not found";
    case AIR_ERR_TYPE:      return "Variable type mismatch";
    case AIR_ERR_NULL:      return "NULL argument";
    default:                return "Unknown error";
    }
}


/* ================================================================
 * AVIATION MATH  — ported from Step5Writer.c
 * ================================================================ */



static double air_sin(double x) {
    double x3 = x * x * x, x5 = x3 * x * x, x7 = x5 * x * x;
    return x - x3 / 6.0 + x5 / 120.0 - x7 / 5040.0;
}
static double air_cos(double x) {
    double x2 = x * x, x4 = x2 * x2, x6 = x4 * x2;
    return 1.0 - x2 / 2.0 + x4 / 24.0 - x6 / 720.0;
}
static double air_sqrt(double n) {
    double x = n; int i;
    if (n <= 0) return 0;
    for (i = 0; i < 10; i++) x = 0.5 * (x + n / x);
    return x;
}
static double air_atan(double x) {
    double r, xp, xs;
    int i;
    if (x > 1.0) return AIR_PI / 2.0 - air_atan(1.0 / x);
    if (x < -1.0) return -AIR_PI / 2.0 - air_atan(1.0 / x);
    r = x; xp = x; xs = x * x;
    for (i = 1; i < 6; i++) {
        xp *= xs;
        if (i % 2 == 1) r -= xp / (2 * i + 1);
        else        r += xp / (2 * i + 1);
    }
    return r;
}
static double air_atan2(double y, double x) {
    if (x > 0)           return air_atan(y / x);
    if (x < 0 && y >= 0) return AIR_PI + air_atan(y / x);
    if (x < 0 && y < 0) return -AIR_PI + air_atan(y / x);
    if (x == 0 && y > 0) return AIR_PI / 2.0;
    if (x == 0 && y < 0) return -AIR_PI / 2.0;
    return 0;
}
static double air_torad(double deg) { return deg * (AIR_PI / 180.0); }

double AIR_GreatCircleNM(double lat1, double lon1,double lat2, double lon2) {
    double dlat = air_torad(lat2 - lat1);
    double dlon = air_torad(lon2 - lon1);
    double rl1 = air_torad(lat1);
    double rl2 = air_torad(lat2);
    double sd2 = air_sin(dlat / 2.0);
    double sl2 = air_sin(dlon / 2.0);
    double a = sd2 * sd2 + air_cos(rl1) * air_cos(rl2) * sl2 * sl2;
    double c = 2.0 * air_atan2(air_sqrt(a), air_sqrt(1.0 - a));
    return AIR_EARTH_KM * c * AIR_KM_TO_NM;
}

AIR_WindComponents AIR_CalcWind(double wind_dir,double wind_speed,double rwy_hdg) {
    AIR_WindComponents r;
    double diff = wind_dir - rwy_hdg;
    while (diff > 180.0) diff -= 360.0;
    while (diff <= -180.0) diff += 360.0;
    r.headwind = wind_speed * air_cos(diff * (AIR_PI / 180.0));
    r.crosswind = wind_speed * air_sin(diff * (AIR_PI / 180.0));
    if (r.crosswind < 0) r.crosswind = -r.crosswind;
    return r;
}

AIR_METARResult AIR_ParseMETAR(const char* raw) {
    
    AIR_METARResult r;
    const char* p, * kt;
    memset(&r, 0, sizeof(r));
    r.wind_dir = -1;
    if (!raw) return r;

    /* copy raw string */
    strncpy(r.raw, raw, sizeof(r.raw) - 1);

    //station — second word after optional "METAR " prefix 
    p = raw;
    if (strncmp(p, "METAR ", 6) == 0) p += 6;
    while (*p && isspace((unsigned char)*p)) p++;
    { int i = 0; while (*p && !isspace((unsigned char)*p) && i < 7) r.station[i++] = *p++; }

    //wind — find KT
    kt = strstr(raw, "KT");
    if (kt) {
        const char* ws = kt;
        while (ws > raw && !isspace((unsigned char)*(ws - 1))) ws--;
        if (isdigit((unsigned char)ws[0]) &&
            isdigit((unsigned char)ws[1]) &&
            isdigit((unsigned char)ws[2])) {
            r.wind_dir = (ws[0] - '0') * 100 + (ws[1] - '0') * 10 + (ws[2] - '0');
        }
        if (isdigit((unsigned char)ws[3]) && isdigit((unsigned char)ws[4]))
            r.wind_speed_kt = (ws[3] - '0') * 10 + (ws[4] - '0');
        // gust
        { const char* g = ws + 5; while (g < kt) { if (*g == 'G' && isdigit((unsigned char)g[1]) && isdigit((unsigned char)g[2])) { r.wind_gust_kt = (g[1] - '0') * 10 + (g[2] - '0'); break; } g++; } }
    }

    //visibility SM
    {
        const char* sm = strstr(raw, "SM");
        if (sm) {
            const char* ve = sm, * vs = ve - 1;
            while (vs > raw && (isdigit((unsigned char)*vs) || *vs == '/')) vs--;
            if (!isdigit((unsigned char)*vs) && *vs != '/') vs++;
            {
                char buf[16] = { 0 }; int i = 0;
                while (vs < ve && i < 15) buf[i++] = *vs++;
                r.visibility = atof(buf);
            }
        }
    }

    // temp/dewpoin
    {
        const char* sl = strchr(raw, '/');
        if (sl) {
            const char* ts = sl - 1;
            while (ts > raw && (isdigit((unsigned char)*ts) || *ts == 'M')) ts--;
            if (!isdigit((unsigned char)*ts) && *ts != 'M') ts++;
            {
                int neg = 0, val = 0;
                if (*ts == 'M') { neg = 1;ts++; }
                if (isdigit((unsigned char)ts[0])) {
                    val = ts[0] - '0';
                    if (isdigit((unsigned char)ts[1])) val = val * 10 + (ts[1] - '0');
                }
                r.temp_c = neg ? -val : val;
            }
            {
                const char* ds = sl + 1; int neg = 0, val = 0;
                if (*ds == 'M') { neg = 1;ds++; }
                if (isdigit((unsigned char)ds[0])) {
                    val = ds[0] - '0';
                    if (isdigit((unsigned char)ds[1])) val = val * 10 + (ds[1] - '0');
                }
                r.dewpoint_c = neg ? -val : val;
            }
        }
    }

    // altimeter A2995 
    {
        const char* ap = raw;
        while (*ap) {
            if (*ap == 'A' && isdigit((unsigned char)ap[1]) && isdigit((unsigned char)ap[2]) && isdigit((unsigned char)ap[3]) && isdigit((unsigned char)ap[4])) {
                int v = (ap[1] - '0') * 1000 + (ap[2] - '0') * 100 + (ap[3] - '0') * 10 + (ap[4] - '0');
                r.altimeter_inhg = v / 100.0; break;
            } ap++;
        }
    }

    r.valid = 1;
    return r;
}

/* validation utils */
int AIR_ValidateICAO(const char* code) {
    int i;
    if (!code || strlen(code) != 4) return 0;
    for (i = 0;i < 4;i++) if (!isalpha((unsigned char)code[i])) return 0;
    return 1;
}
int AIR_IsAircraftIdentifier(const char* v) {
    int len; if (!v) return 0; len = (int)strlen(v);
    if (len == 5 && isalpha((unsigned char)v[0]) && isalpha((unsigned char)v[1]) &&
        isdigit((unsigned char)v[2]) && isdigit((unsigned char)v[3]) && isdigit((unsigned char)v[4])) return 1;
    if (len == 6 && isalpha((unsigned char)v[0]) && v[1] == '-' &&
        isalnum((unsigned char)v[2]) && isalnum((unsigned char)v[3]) &&
        isalnum((unsigned char)v[4]) && isalnum((unsigned char)v[5])) return 1;
    return 0;
}
int AIR_IsCoordFormat(const char* v) {
    const char* c; char tmp[128]; char* ls, * lo, * lp, * pp;
    if (!v || !strchr(v, ',')) return 0;
    strncpy(tmp, v, 127); tmp[127] = '\0';
    c = strchr(tmp, ','); if (!c) return 0;
    *(char*)c = '\0'; ls = tmp; lo = (char*)c + 1;
    while (isspace((unsigned char)*ls)) ls++;
    while (isspace((unsigned char)*lo)) lo++;
    lp = ls; if (*lp == '-') lp++;
    if (!isdigit((unsigned char)*lp)) return 0;
    while (*lp && (isdigit((unsigned char)*lp) || *lp == '.')) lp++;
    if (*lp && !isspace((unsigned char)*lp)) return 0;
    pp = lo; if (*pp == '-') pp++;
    if (!isdigit((unsigned char)*pp)) return 0;
    while (*pp && (isdigit((unsigned char)*pp) || *pp == '.')) pp++;
    if (*pp && !isspace((unsigned char)*pp)) return 0;
    return 1;
}

/* FetchMETAR stub — real implementation needs libcurl */
AIR_METARResult AIR_FetchMETAR(AIR_Runtime* rt, const char* icao) {
    AIR_METARResult r; memset(&r, 0, sizeof(r)); r.wind_dir = -1;
    (void)rt; (void)icao;
#ifndef AIRLANG_NO_CURL
    /* TODO:  MetarFetcher.c fetch_metar_from_api()  */
#endif
    return r;
}
