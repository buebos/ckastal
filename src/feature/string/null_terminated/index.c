#ifndef __CKASTAL_DSA_STRING_NULL_TERMINATED_C__
#define __CKASTAL_DSA_STRING_NULL_TERMINATED_C__

char ck_char_to_lower(char c) {
    if (c >= 'A' && c <= 'Z') {
        return c + ('a' - 'A');
    }
    return c;
}

int ck_string_null_insensitive_compare(char* s1, char* s2) {
    while (*s1 && *s2) {
        if (ck_char_to_lower(*s1) != ck_char_to_lower(*s2)) {
            return 1;
        }

        s1++;
        s2++;
    }
    return (*s1 == *s2) ? 0 : 1;
}

#endif /* __CKASTAL_DSA_STRING_NULL_TERMINATED_C__ */
