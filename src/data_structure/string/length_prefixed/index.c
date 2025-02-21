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
#define ck_lenstr_slp(str) str, sizeof(str) - 1, sizeof(str) - 1

typedef struct Ck_LenStr {
    size_t _length;
    size_t _capacity;
    char* _str;
} Ck_LenStr;

Ck_LenStr ck_lenstr_init(char* data, size_t capacity, size_t length) {
    Ck_LenStr str = {
        ._capacity = capacity,
        ._length = length,
        ._str = data,
    };

    return str;
}

size_t ck_lenstr_space(Ck_LenStr* self) {
    return self->_capacity - self->_length;
}

void ck_lenstr_pushc(Ck_LenStr* self, char c) {
    self->_str[self->_length] = c;
    self->_length += 1;
}

void ck_lenstr_pushs(Ck_LenStr* self, char* str, size_t str_length) {
    for (size_t i = 0; i < str_length; i++) {
        ck_lenstr_pushc(self, str[i]);
    }
}

void ck_lenstr_copy(Ck_LenStr* self, char* str, size_t str_length) {
    for (size_t i = 0; i < str_length; i++) {
        self->_str[i] = str[i];
    }

    self->_length = str_length;
}

/**
 * Does not copy the string, just makes a struct with the start set to
 * the source original pointer + the start parameter and length with
 * the difference of start and end.
 */
Ck_LenStr ck_lenstr_sliced(Ck_LenStr* source, size_t start, size_t end) {
    if (start >= end) {
        return ck_lenstr_init(NULL, 0, 0);
    }

    return ck_lenstr_init(source->_str + start, end - start, end - start);
}

/**
 * Tries to mimic the functionality of `strcmp` of C's standard
 * library.
 *
 * @ref strcmp
 */
int ck_lenstr_cmp(Ck_LenStr* a, Ck_LenStr* b) {
    if (a->_length < b->_length) {
        return -1;
    }
    if (b->_length < a->_length) {
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

    while (a->_str[i] == b->_str[i] && i < a->_length - 1) {
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
    return a->_length == b->_length && (((a == b || a->_length == b->_length) && a->_str == b->_str) || ck_lenstr_cmp(a, b) == 0);
}

#endif /* __CKASTAL_STRING_LENGTH_PREFIXED_C__ */
