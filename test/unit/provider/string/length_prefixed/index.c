#include "../../../../../lib/provider/string/length_prefixed/index.c"

#include <assert.h>
#include <string.h>

#include "../../../../../vendor/clogg/src/clogg.c"

int main(void) {
    Ck_LenStr a = ck_lenstr_init(ck_lenstr_slp("hello"));
    Ck_LenStr b = ck_lenstr_init(ck_lenstr_slp("hello"));

    assert(ck_lenstr_cmp(&a, &b) == 0);
    assert(ck_lenstr_eq(&a, &b));

    ck_lenstr_set(&b, ck_lenstr_slp("world"));

    assert(ck_lenstr_cmp(&a, &b) == strcmp("hello", "world"));
    assert(!ck_lenstr_eq(&a, &b));

    ck_lenstr_set(&b, ck_lenstr_slp("hello world"));
    assert(ck_lenstr_cmp(&a, &b) == strcmp("hello", "hello world"));
    assert(!ck_lenstr_eq(&a, &b));

    ck_lenstr_set(&b, ck_lenstr_slp(""));
    assert(ck_lenstr_cmp(&a, &b) == strcmp("hello", ""));
    assert(!ck_lenstr_eq(&a, &b));

    ck_lenstr_set(&b, NULL, 0);
    assert(ck_lenstr_cmp(&a, &b) == strcmp("hello", ""));
    assert(!ck_lenstr_eq(&a, &b));

    ck_lenstr_set(&a, NULL, 0);
    ck_lenstr_set(&b, NULL, 0);
    assert(ck_lenstr_cmp(&a, &b) == strcmp("", ""));
    assert(ck_lenstr_eq(&a, &b));

    ck_lenstr_set(&a, NULL, 0);
    ck_lenstr_set(&b, ck_lenstr_slp("hello"));
    assert(ck_lenstr_cmp(&a, &b) == -1);
    assert(!ck_lenstr_eq(&a, &b));

    ck_lenstr_set(&a, ck_lenstr_slp("h"));
    ck_lenstr_set(&b, ck_lenstr_slp("h"));
    assert(ck_lenstr_cmp(&a, &b) == 0);
    assert(ck_lenstr_eq(&a, &b));

    ck_lenstr_set(&a, ck_lenstr_slp("aaaaaaaaaaaaaaaa"));
    ck_lenstr_set(&b, ck_lenstr_slp("aaaaaaaaaaaaaaaa"));
    assert(ck_lenstr_cmp(&a, &b) == 0);
    assert(ck_lenstr_eq(&a, &b));

    ck_lenstr_set(&b, ck_lenstr_slp("aaaaaaaaaaaaaaaab"));
    assert(ck_lenstr_cmp(&a, &b) == -1);
    assert(!ck_lenstr_eq(&a, &b));

    ck_lenstr_set(&a, NULL, 0);
    ck_lenstr_set(&b, "", 0);
    assert(ck_lenstr_cmp(&a, &b) != strcmp("", ""));

    ck_lenstr_set(&a, "", 0);
    ck_lenstr_set(&b, "", 0);
    assert(ck_lenstr_cmp(&a, &b) == strcmp("", ""));

    ck_lenstr_set(&a, ck_lenstr_slp("hello"));
    ck_lenstr_set(&b, ck_lenstr_slp("h"));
    Ck_LenStr slice = ck_lenstr_slice(&a, 0, 1);
    assert(ck_lenstr_eq(&slice, &b));

    ck_lenstr_set(&a, ck_lenstr_slp("hello world"));
    ck_lenstr_set(&b, ck_lenstr_slp("hello"));
    slice = ck_lenstr_slice(&a, 0, 5);
    assert(ck_lenstr_cmp(&slice, &b) == 0);
    assert(ck_lenstr_eq(&slice, &b));

    ck_lenstr_set(&a, ck_lenstr_slp("hello world"));
    ck_lenstr_set(&b, ck_lenstr_slp("hello universe"));
    slice = ck_lenstr_slice(&a, 0, 5);
    assert(ck_lenstr_cmp(&slice, &b) != 0);
    assert(!ck_lenstr_eq(&slice, &b));

    char buffer[64] = {0};

    snprintf(buffer, 64, "%.*s", a._len, a._str);

    for (size_t i = 0; i < a._len; i++) {
        assert(buffer[i] == a._str[i]);
    }

    cloggln(CLOGG_INFO, "All tests passed");

    return 0;
}
