#ifndef __CKASTAL_VALIDATOR_C__
#define __CKASTAL_VALIDATOR_C__

#include <regex.h>

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
    regex_t regex;

    result.status = !!regcomp(&regex, "^[+-]?([0-9]+([.][0-9]*)?|[.][0-9]+)([eE][+-]?[0-9]+)?$", REG_EXTENDED);

    if (result.status != CK_VALIDATION_OK) {
        result.message = "Error";
        regfree(&regex);
        return result;
    }

    result.status = !!regexec(&regex, value, (size_t)0, NULL, 0);

    if (result.status != CK_VALIDATION_OK) {
        result.message = "Error";
    }

    regfree(&regex);

    return result;
}

#endif /* __CKASTAL_VALIDATOR_C__ */