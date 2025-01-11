#ifndef __CLOGG_LOG_H__

#define __CLOGG_LOG_H__

#include <stdarg.h>
#include <stdio.h>

#include "color.h"

#ifdef _CLOGG_DEV

#define cloggdev(message, ...) clogg(CLOGG_LOG_LEVEL_COUNT, message, ##__VA_ARGS__)

#else

#define cloggdev(message, ...)

#endif /** CLOGG_DEV */

#define CLOGG_LOG_LEVEL_COUNT 3

extern const char *CLOGG_PREFIXES[];

typedef enum Clogg_LogLevel {
    CLOGG_INFO = 0,
    CLOGG_WARN,
    CLOGG_ERROR
} Clogg_LogLevel;

void clogg(Clogg_LogLevel log_level, char *msg, ...);
void cloggln(Clogg_LogLevel log_level, char *msg, ...);

#endif /** __CLOGG_LOG_H__ */
