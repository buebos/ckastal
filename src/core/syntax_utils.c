#ifndef __CKASTAL_SYNTAX_UTILS_H__
#define __CKASTAL_SYNTAX_UTILS_H__

#define foreach(type, node, iterator) \
    for (type node = (type)iterator.next(&iterator); node != NULL; node = (type)iterator.next(&iterator))

#define cast(ptr, type) ((type)(ptr))
#define deref(ptr, type) (*(type*)(ptr))

#define _loop_string(seq) _loop_string_end(_loop_string_a seq)
#define _loop_string_a(x) #x, _loop_string_b
#define _loop_string_b(x) #x, _loop_string_a
#define _loop_string_a_end
#define _loop_string_b_end
#define _loop_string_end(...) _loop_string_end_(__VA_ARGS__)
#define _loop_string_end_(...) __VA_ARGS__##_end

#define _loop_enum(seq) _loop_enum_end(_loop_enum_a seq)
#define _loop_enum_a(x) x, _loop_enum_b
#define _loop_enum_b(x) x, _loop_enum_a
#define _loop_enum_a_end
#define _loop_enum_b_end
#define _loop_enum_end(...) _loop_enum_end_(__VA_ARGS__)
#define _loop_enum_end_(...) __VA_ARGS__##_end

#define enum_str_setup(alias, prefix, str_array_id, str_fn_id, first_enum_member_name, seq) \
    typedef enum alias { first_enum_member_name = 0,                                        \
                         _loop_enum(seq) } alias;                                           \
    const char* str_array_id[] = {#first_enum_member_name, _loop_string(seq)};              \
    const char* str_fn_id(alias type) {                                                     \
        if (type < 0 || type >= sizeof(str_array_id)) {                                     \
            return "UNKNOWN";                                                               \
        }                                                                                   \
        return str_array_id[type] + sizeof(prefix);                                         \
    }

#endif /** __CKASTAL_SYNTAX_UTILS_H__ */
