#include "../include/clogg.h"

const char *CLOGG_PREFIXES[] = {
    "INFO",
    "WARN",
    "ERROR",
    "DEBUG",
};
const char *COLORS[] = {
    AC_GREEN,
    AC_YELLOW,
    AC_RED,
    AC_PURPLE,
};

void _clogg_va(Clogg_LogLevel log_level, char *msg, va_list params) {
    printf(
        "%s[%s]: " AC_CYAN,
        COLORS[log_level],
        CLOGG_PREFIXES[log_level]

    );

    if (log_level < 2) {
        printf(" ");
    }

    vprintf(msg, params);
    printf(AC_RESET);
}

void clogg(Clogg_LogLevel log_level, char *msg, ...) {
    va_list params;
    va_start(params, msg);
    _clogg_va(log_level, msg, params);
    va_end(params);
}

void cloggln(Clogg_LogLevel log_level, char *msg, ...) {
    va_list params;
    va_start(params, msg);
    _clogg_va(log_level, msg, params);
    va_end(params);

    printf("\n");
}
