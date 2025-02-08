#ifndef __CKASTAL_STRING_LENGTH_PREFIXED_C__
#define __CKASTAL_STRING_LENGTH_PREFIXED_C__

#include "../../../core/init.c"

/**
 * Macro helper to pass in the parameters for a C literal string with it's
 * length unwrapped and divided by a comma. The word 'lps' stands for
 * 'string literal parameter'. The idea is too use it as follows:
 *
```c
ck_lenstr_init(ck_lenstr_slp("This a long literal string example"))
// Is the equivalent to
ck_lenstr_init("This a long literal string example", 34)
```
 *
 * This avoids the need to count the chars for the literal string manually
 * while coding (or select them with the cursor to count them). It's just
 * a minor syntactic sugar lol.
 */
#define ck_lenstr_slp(str) str, sizeof(str) - 1

typedef struct Ck_LenStr {
    size_t _len;
    char* _str;
} Ck_LenStr;

Ck_LenStr ck_lenstr_init(char* data, size_t length) {
    Ck_LenStr str = {
        ._len = length,
        ._str = data,
    };

    return str;
}

void ck_lenstr_set(Ck_LenStr* str, char* data, size_t length) {
    str->_len = length;
    str->_str = data;
}

/**
 * Tries to mimic the functionality of `strcmp` of C's standard
 * library.
 *
 * @ref strcmp
 */
char ck_lenstr_cmp(Ck_LenStr* a, Ck_LenStr* b) {
    if (a->_len < b->_len) {
        return -1;
    }
    if (b->_len < a->_len) {
        return 1;
    }
    if (a == b || a->_str == b->_str) {
        return 0;
    }
    if (NULL == a->_str && b->_str) {
        return -1;
    }
    if (NULL == b->_str && a->_str) {
        return 1;
    }

    size_t i = 0;

    while (a->_str[i] == b->_str[i] && i < a->_len - 1) {
        i += 1;
    }

    if (a->_str[i] > b->_str[i]) {
        return 1;
    }
    if (a->_str[i] < b->_str[i]) {
        return -1;
    }

    return 0;
}

/**
 * Minor optimization of `ck_lenstr_cmp`, to avoid iterating over the chars;
 * if the strings differ in length the function directly returns `false`.
 *
 * @ref ck_lenstr_cmp
 */
bool ck_lenstr_eq(Ck_LenStr* a, Ck_LenStr* b) {
    return a->_len == b->_len && (a == b || a->_len == b->_len && a->_str == b->_str || ck_lenstr_cmp(a, b) == 0);
}

/**
 * Does not copy the string, just makes a struct with the start set to
 * the source original pointer + the start parameter and length with
 * the difference of start and end.
 */
Ck_LenStr ck_lenstr_slice(Ck_LenStr* source, size_t start, size_t end) {
    if (start >= end) {
        return ck_lenstr_init(NULL, 0);
    }

    return ck_lenstr_init(source->_str + start, end - start);
}

void ck_lenstr_copy(Ck_LenStr* source, Ck_LenStr* destination) {
    for (size_t i = 0; i < source->_len; i++) {
        destination->_str[i] = source->_str[i];
    }

    destination->_len = source->_len;
}

#endif /* __CKASTAL_STRING_LENGTH_PREFIXED_C__ */
