#ifndef __CKASTAL_VALIDATOR_C__
#define __CKASTAL_VALIDATOR_C__

#include "../core/os.c"

#ifdef CKASTAL_CORE_OS_WINDOWS
#include "../../include/gnu_win_regex.c"
#else
#include <regex.h>
#endif

#include "../core/init.c"

typedef enum Ck_ValidationStatus {
    CK_VALIDATION_OK = 0,
    CK_VALIDATION_ERROR,
    CK_VALIDATION_WARN,
} Ck_ValidationStatus;

typedef struct Ck_ValidationRes {
    Ck_ValidationStatus status;
    char* message;
} Ck_ValidationRes;

Ck_ValidationRes ck_validation_float(char* value) {
    Ck_ValidationRes result = {
        .status = CK_VALIDATION_OK,
        .message = "Success",
    };

    regex_t valid_regex, incomplete_regex;

    const char* valid_pattern = "^[+-]?([0-9]+([.][0-9]*)?|[.][0-9]+)([eE][+-]?[0-9]+)?$";
    const char* incomplete_pattern = "^[+-]?([0-9]+([.][0-9]*)?|[.][0-9]+)[eE][+-]?$";

    if (regcomp(&valid_regex, valid_pattern, REG_EXTENDED) ||
        regcomp(&incomplete_regex, incomplete_pattern, REG_EXTENDED)) {
        result.status = CK_VALIDATION_ERROR;
        result.message = "Regex compilation error";
        return result;
    }

    if (regexec(&valid_regex, value, 0, NULL, 0) == 0) {
        result.status = CK_VALIDATION_OK;
        result.message = "Success";
    } else if (regexec(&incomplete_regex, value, 0, NULL, 0) == 0) {
        result.status = CK_VALIDATION_WARN;
        result.message = "Warning: Incomplete float";
    } else {
        result.status = CK_VALIDATION_ERROR;
        result.message = "Error: Invalid float";
    }

    regfree(&valid_regex);
    regfree(&incomplete_regex);

    return result;
}

#endif /* __CKASTAL_VALIDATOR_C__ */