#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "../../../include/prefixed_length_string.c"
#include "../../../vendor/clogg/src/clogg.c"

int main(void) {
    Ck_LenStr hello = ck_lenstr_init(ck_lenstr_slp("hello"));
    Ck_LenStr hello__2 = ck_lenstr_init(malloc(5 * sizeof(char)), 5, 0);

    ck_lenstr_pushs(&hello__2, "hello", 5);

    assert(ck_lenstr_cmp(&hello, &hello) == 0);
    assert(ck_lenstr_eq(&hello, &hello));
    assert(ck_lenstr_cmp(&hello, &hello__2) == 0);
    assert(ck_lenstr_eq(&hello, &hello__2));
    assert(ck_lenstr_cmp(&hello__2, &hello) == 0);
    assert(ck_lenstr_eq(&hello__2, &hello));

    Ck_LenStr world = ck_lenstr_init(ck_lenstr_slp("world"));

    assert(ck_lenstr_cmp(&hello, &world) == strcmp("hello", "world"));
    assert(!ck_lenstr_eq(&hello, &world));

    Ck_LenStr hello_world = ck_lenstr_init(ck_lenstr_slp("hello world"));
    assert(ck_lenstr_cmp(&hello, &hello_world) == strcmp("hello", "hello world"));
    assert(!ck_lenstr_eq(&hello, &hello_world));

    Ck_LenStr empty = ck_lenstr_init(ck_lenstr_slp(""));
    assert(ck_lenstr_cmp(&hello, &empty) == strcmp("hello", ""));
    assert(!ck_lenstr_eq(&hello, &empty));

    Ck_LenStr null = ck_lenstr_init(NULL, 0, 0);
    assert(ck_lenstr_cmp(&hello, &null) == strcmp("hello", ""));
    assert(!ck_lenstr_eq(&hello, &null));

    Ck_LenStr null2 = ck_lenstr_init(NULL, 0, 0);
    assert(ck_lenstr_cmp(&null, &null2) == strcmp("", ""));
    assert(ck_lenstr_eq(&null, &null2));

    assert(ck_lenstr_cmp(&null, &hello__2) == -1);
    assert(!ck_lenstr_eq(&null, &hello__2));

    /**
     * Null string should not be the same as empty string.
     */
    assert(ck_lenstr_cmp(&null, &empty) != strcmp("", ""));

    Ck_LenStr empty__2 = ck_lenstr_init(ck_lenstr_slp(""));
    assert(ck_lenstr_cmp(&empty, &empty__2) == strcmp("", ""));

    Ck_LenStr h = ck_lenstr_init(ck_lenstr_slp("h"));
    Ck_LenStr h__2 = ck_lenstr_init(ck_lenstr_slp("h"));
    assert(ck_lenstr_cmp(&h, &h__2) == 0);
    assert(ck_lenstr_eq(&h, &h__2));

    Ck_LenStr a_times_16 = ck_lenstr_init(ck_lenstr_slp("aaaaaaaaaaaaaaaa"));
    Ck_LenStr a_times_16__2 = ck_lenstr_init(ck_lenstr_slp("aaaaaaaaaaaaaaaa"));
    assert(ck_lenstr_cmp(&a_times_16, &a_times_16__2) == 0);
    assert(ck_lenstr_eq(&a_times_16, &a_times_16__2));

    Ck_LenStr a_times_16_b_end = ck_lenstr_init(ck_lenstr_slp("aaaaaaaaaaaaaaaab"));
    assert(ck_lenstr_cmp(&a_times_16, &a_times_16_b_end) == -1);
    assert(!ck_lenstr_eq(&a_times_16, &a_times_16_b_end));

    Ck_LenStr hello__sliced = ck_lenstr_sliced(&hello, 0, 1);
    assert(ck_lenstr_eq(&hello__sliced, &h));

    Ck_LenStr hello_world__sliced = ck_lenstr_sliced(&hello_world, 0, 5);
    assert(ck_lenstr_cmp(&hello_world__sliced, &hello) == 0);
    assert(ck_lenstr_eq(&hello_world__sliced, &hello));

    Ck_LenStr hello_universe = ck_lenstr_init(ck_lenstr_slp("hello universe"));
    assert(ck_lenstr_cmp(&hello_world__sliced, &hello_universe) != 0);
    assert(!ck_lenstr_eq(&hello_world__sliced, &hello_universe));

    char buffer[64] = {0};

    Ck_LenStr empty__buffer = ck_lenstr_init(buffer, 64, 0);
    assert(ck_lenstr_cmp(&empty, &empty__buffer) != 0);
    assert(!ck_lenstr_eq(&empty, &empty__buffer));
    assert(ck_lenstr_cmp(&empty__2, &empty__buffer) != 0);
    assert(!ck_lenstr_eq(&empty__2, &empty__buffer));

    snprintf(buffer, 64, "%.*s", (int)hello_world._length, hello_world._str);

    for (size_t i = 0; i < hello_world._length; i++) {
        assert(buffer[i] == hello_world._str[i]);
    }

    cloggln(CLOGG_INFO, "All tests passed");

    return 0;
}
