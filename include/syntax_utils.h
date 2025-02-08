#ifndef __CKASTAL_SYNTAX_UTILS_H__
#define __CKASTAL_SYNTAX_UTILS_H__

#define foreach(type, node, iterator) \
    for (type node = (type)iterator.next(&iterator); node != NULL; node = (type)iterator.next(&iterator))

#define cast(ptr, type) ((type)(ptr))
#define deref(ptr, type) (*(type *)(ptr))

#endif /** __CKASTAL_SYNTAX_UTILS_H__ */
