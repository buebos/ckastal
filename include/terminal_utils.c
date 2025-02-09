#ifndef __CKASTAL_TERMINAL_UTILS_C__
#define __CKASTAL_TERMINAL_UTILS_C__

/** To call the correct system clear console */
#ifdef __unix__
#elif defined(_WIN32) || defined(WIN32)
#define OS_Windows
#endif

#include <stdlib.h>

/** Clears the terminal */
void clear() {
#ifdef OS_Windows
    system("cls");
#else
    system("clear");
#endif
}

#endif /** __CKASTAL_TERMINAL_UTILS_C__ */
