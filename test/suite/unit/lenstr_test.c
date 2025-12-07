#include <stdlib.h>
#include <string.h>

#include "../../../include/prefixed_length_string.c"
#include "../../../vendor/ckester/include/index.c"

TEST(lenstr_basic_equality) {
    Ck_LenStr hello = ck_lenstr_init(ck_lenstr_slp("hello"));
    Ck_LenStr hello__2 = ck_lenstr_init(malloc(5 * sizeof(char)), 5, 0);
    ck_lenstr_pushs(&hello__2, "hello", 5);

    ASSERT_EQ(ck_lenstr_cmp(&hello, &hello), 0);
    ASSERT_TRUE(ck_lenstr_eq(&hello, &hello));
    ASSERT_EQ(ck_lenstr_cmp(&hello, &hello__2), 0);
    ASSERT_TRUE(ck_lenstr_eq(&hello, &hello__2));
    ASSERT_EQ(ck_lenstr_cmp(&hello__2, &hello), 0);
    ASSERT_TRUE(ck_lenstr_eq(&hello__2, &hello));

    free(hello__2._str);
}

TEST(lenstr_inequality) {
    Ck_LenStr hello = ck_lenstr_init(ck_lenstr_slp("hello"));
    Ck_LenStr world = ck_lenstr_init(ck_lenstr_slp("world"));

    ASSERT_EQ(ck_lenstr_cmp(&hello, &world), strcmp("hello", "world"));
    ASSERT_FALSE(ck_lenstr_eq(&hello, &world));
}

TEST(lenstr_different_lengths) {
    Ck_LenStr hello = ck_lenstr_init(ck_lenstr_slp("hello"));
    Ck_LenStr hello_world = ck_lenstr_init(ck_lenstr_slp("hello world"));

    ASSERT_EQ(ck_lenstr_cmp(&hello, &hello_world), strcmp("hello", "hello world"));
    ASSERT_FALSE(ck_lenstr_eq(&hello, &hello_world));
}

TEST(lenstr_empty_string) {
    Ck_LenStr hello = ck_lenstr_init(ck_lenstr_slp("hello"));
    Ck_LenStr empty = ck_lenstr_init(ck_lenstr_slp(""));

    ASSERT_EQ(ck_lenstr_cmp(&hello, &empty), strcmp("hello", ""));
    ASSERT_FALSE(ck_lenstr_eq(&hello, &empty));
}

TEST(lenstr_null_string) {
    Ck_LenStr hello = ck_lenstr_init(ck_lenstr_slp("hello"));
    Ck_LenStr null = ck_lenstr_init(NULL, 0, 0);

    ASSERT_EQ(ck_lenstr_cmp(&hello, &null), strcmp("hello", ""));
    ASSERT_FALSE(ck_lenstr_eq(&hello, &null));
}

TEST(lenstr_null_equality) {
    Ck_LenStr null = ck_lenstr_init(NULL, 0, 0);
    Ck_LenStr null2 = ck_lenstr_init(NULL, 0, 0);

    ASSERT_EQ(ck_lenstr_cmp(&null, &null2), strcmp("", ""));
    ASSERT_TRUE(ck_lenstr_eq(&null, &null2));
}

TEST(lenstr_null_vs_hello) {
    Ck_LenStr null = ck_lenstr_init(NULL, 0, 0);
    Ck_LenStr hello__2 = ck_lenstr_init(malloc(5 * sizeof(char)), 5, 0);
    ck_lenstr_pushs(&hello__2, "hello", 5);

    ASSERT_EQ(ck_lenstr_cmp(&null, &hello__2), -1);
    ASSERT_FALSE(ck_lenstr_eq(&null, &hello__2));

    free(hello__2._str);
}

TEST(lenstr_null_vs_empty) {
    Ck_LenStr null = ck_lenstr_init(NULL, 0, 0);
    Ck_LenStr empty = ck_lenstr_init(ck_lenstr_slp(""));

    /* Null string should not be the same as empty string. */
    ASSERT_NEQ(ck_lenstr_cmp(&null, &empty), strcmp("", ""));
}

TEST(lenstr_empty_equality) {
    Ck_LenStr empty = ck_lenstr_init(ck_lenstr_slp(""));
    Ck_LenStr empty__2 = ck_lenstr_init(ck_lenstr_slp(""));

    ASSERT_EQ(ck_lenstr_cmp(&empty, &empty__2), strcmp("", ""));
}

TEST(lenstr_single_char) {
    Ck_LenStr h = ck_lenstr_init(ck_lenstr_slp("h"));
    Ck_LenStr h__2 = ck_lenstr_init(ck_lenstr_slp("h"));

    ASSERT_EQ(ck_lenstr_cmp(&h, &h__2), 0);
    ASSERT_TRUE(ck_lenstr_eq(&h, &h__2));
}

TEST(lenstr_repeated_chars) {
    Ck_LenStr a_times_16 = ck_lenstr_init(ck_lenstr_slp("aaaaaaaaaaaaaaaa"));
    Ck_LenStr a_times_16__2 = ck_lenstr_init(ck_lenstr_slp("aaaaaaaaaaaaaaaa"));

    ASSERT_EQ(ck_lenstr_cmp(&a_times_16, &a_times_16__2), 0);
    ASSERT_TRUE(ck_lenstr_eq(&a_times_16, &a_times_16__2));
}

TEST(lenstr_repeated_chars_with_diff) {
    Ck_LenStr a_times_16 = ck_lenstr_init(ck_lenstr_slp("aaaaaaaaaaaaaaaa"));
    Ck_LenStr a_times_16_b_end = ck_lenstr_init(ck_lenstr_slp("aaaaaaaaaaaaaaaab"));

    ASSERT_EQ(ck_lenstr_cmp(&a_times_16, &a_times_16_b_end), -1);
    ASSERT_FALSE(ck_lenstr_eq(&a_times_16, &a_times_16_b_end));
}

TEST(lenstr_slicing_single_char) {
    Ck_LenStr hello = ck_lenstr_init(ck_lenstr_slp("hello"));
    Ck_LenStr h = ck_lenstr_init(ck_lenstr_slp("h"));
    Ck_LenStr hello__sliced = ck_lenstr_sliced(&hello, 0, 1);

    ASSERT_TRUE(ck_lenstr_eq(&hello__sliced, &h));
}

TEST(lenstr_slicing_prefix) {
    Ck_LenStr hello = ck_lenstr_init(ck_lenstr_slp("hello"));
    Ck_LenStr hello_world = ck_lenstr_init(ck_lenstr_slp("hello world"));
    Ck_LenStr hello_world__sliced = ck_lenstr_sliced(&hello_world, 0, 5);

    ASSERT_EQ(ck_lenstr_cmp(&hello_world__sliced, &hello), 0);
    ASSERT_TRUE(ck_lenstr_eq(&hello_world__sliced, &hello));
}

TEST(lenstr_slicing_comparison) {
    Ck_LenStr hello_world = ck_lenstr_init(ck_lenstr_slp("hello world"));
    Ck_LenStr hello_universe = ck_lenstr_init(ck_lenstr_slp("hello universe"));
    Ck_LenStr hello_world__sliced = ck_lenstr_sliced(&hello_world, 0, 5);

    ASSERT_NEQ(ck_lenstr_cmp(&hello_world__sliced, &hello_universe), 0);
    ASSERT_FALSE(ck_lenstr_eq(&hello_world__sliced, &hello_universe));
}

TEST(lenstr_buffer_vs_empty) {
    Ck_LenStr empty = ck_lenstr_init(ck_lenstr_slp(""));
    Ck_LenStr empty__2 = ck_lenstr_init(ck_lenstr_slp(""));
    char buffer[64] = {0};
    Ck_LenStr empty__buffer = ck_lenstr_init(buffer, 64, 0);

    ASSERT_NEQ(ck_lenstr_cmp(&empty, &empty__buffer), 0);
    ASSERT_FALSE(ck_lenstr_eq(&empty, &empty__buffer));
    ASSERT_NEQ(ck_lenstr_cmp(&empty__2, &empty__buffer), 0);
    ASSERT_FALSE(ck_lenstr_eq(&empty__2, &empty__buffer));
}

TEST(lenstr_buffer_format) {
    Ck_LenStr hello_world = ck_lenstr_init(ck_lenstr_slp("hello world"));
    char buffer[64] = {0};

    snprintf(buffer, 64, "%.*s", (int)hello_world._length, hello_world._str);

    for (size_t i = 0; i < hello_world._length; i++) {
        ASSERT_EQ(buffer[i], hello_world._str[i]);
    }
}
