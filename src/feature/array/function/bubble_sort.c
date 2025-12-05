#ifndef __CKASTAL_DSA_ARRAY_FUNCTION_BUBBLE_SORT_C__
#define __CKASTAL_DSA_ARRAY_FUNCTION_BUBBLE_SORT_C__

#include "../../../../include/init.c"

#define CK_ARRAY_DECLARE_BUBBLE_SORT_FN(Type, fn_name, logic_block) \
    void fn_name(Type* array, size_t array_len) {                   \
        for (size_t i = 0; i < array_len; i++) {                    \
            bool swapped = false;                                   \
                                                                    \
            for (size_t j = 0; j < array_len - i - 1; j++) {        \
                Type temp;                                          \
                                                                    \
                Type* a = &array[j];                                \
                Type* b = &array[j + 1];                            \
                bool should_swap = false;                           \
                                                                    \
                {                                                   \
                    logic_block                                     \
                }                                                   \
                                                                    \
                if (should_swap) {                                  \
                    temp = *b;                                      \
                    *b = *a;                                        \
                    *a = temp;                                      \
                                                                    \
                    swapped = true;                                 \
                }                                                   \
            }                                                       \
            if (!swapped) {                                         \
                break;                                              \
            }                                                       \
        }                                                           \
    }

#endif /* __CKASTAL_DSA_ARRAY_FUNCTION_BUBBLE_SORT_C__ */