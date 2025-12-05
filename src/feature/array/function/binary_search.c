#ifndef __CKASTAL_DSA_ARRAY_FUNCTION_BINARY_SEARCH_C__
#define __CKASTAL_DSA_ARRAY_FUNCTION_BINARY_SEARCH_C__

#define CK_ARRAY_DECLARE_BINARY_SEARCH_FN(fn_definition, order_logic_block) \
    fn_definition {                                                         \
        size_t min = 0;                                                     \
        size_t max = array_len - 1;                                         \
                                                                            \
        while (min <= max) {                                                \
            size_t index = (min + max) / 2;                                 \
                                                                            \
            int order = 0;                                                  \
                                                                            \
            /**                                                             \
             * This will be the code block with logic regarding             \
             * the comparison.                                              \
             */                                                             \
            order_logic_block;                                              \
                                                                            \
            if (order == 0) {                                               \
                return &array[index];                                       \
            }                                                               \
                                                                            \
            if (order < 0) {                                                \
                max = index - 1;                                            \
            } else {                                                        \
                min = index + 1;                                            \
            }                                                               \
        }                                                                   \
                                                                            \
        return NULL;                                                        \
    }

#endif /* __CKASTAL_DSA_ARRAY_FUNCTION_BINARY_SEARCH_C__ */
