#ifndef __CKASTAL_CORE_SYSTEM_UTILS_C__
#define __CKASTAL_CORE_SYSTEM_UTILS_C__

#include <stdlib.h>

#include "os.c"

/** Clears the terminal */
void clear() {
#ifdef CKASTAL_CORE_OS_WINDOWS
    system("cls");
#else
    system("clear");
#endif
}

#endif /* __CKASTAL_CORE_SYSTEM_UTILS_C__ */