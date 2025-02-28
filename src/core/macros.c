#ifndef __CKASTAL_MACROS_C__
#define __CKASTAL_MACROS_C__

#define CK_GET_LAST_ARG(...) _CK_GET_LAST_ARG_HELPER(__VA_ARGS__, _)
#define _CK_GET_LAST_ARG_HELPER(last, ...) __VA_ARGS__

#define CK_REMOVE_LAST_ARG(...) _CK_REMOVE_LAST_ARG_HELPER(__VA_ARGS__, _)
#define _CK_REMOVE_LAST_ARG_HELPER(last, ...) last

#endif /* __CKASTAL_MACROS_C__ */