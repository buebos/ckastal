/**
 * @file init.c
 * @author Gael Herrera (hrac.gael@gmail.com)
 * @brief The purpose of this file is to provide common macro and type
 * definitions from stdlib with out the need to include it in case the
 * implementation does not wishes to do that.
 * @version 0.1
 * @date 2025-01-04
 *
 * @copyright Copyright (c) 2025
 */

#ifndef __CKASTAL_CORE_INIT_C__
#define __CKASTAL_CORE_INIT_C__

/**
 * size_t definition
 */
#if !defined(_SIZE_T) && !defined(_SIZE_T_DEFINED)
#define _SIZE_T
#define _SIZE_T_DEFINED
typedef unsigned int size_t;
#endif

/**
 * NULL definition
 */
#ifndef NULL
#define NULL ((void *)0)
#endif

/**
 * Allocators
 */
typedef void *(*Ck_AllocOperation)(size_t);
typedef void (*Ck_FreeOperation)(void *);

typedef struct Ck_Memory {
    Ck_AllocOperation alloc;
    Ck_FreeOperation free;
} Ck_Memory;

#ifdef _STDLIB_H_
#include <stdlib.h>

void *ck_calloc(size_t size) {
    return calloc(1, size);
}

Ck_Memory ck_memory = {
    .alloc = ck_calloc,
    .free = free,
};
#else
extern Ck_Memory ck_memory;
#endif

/**
 * Boolean definitions
 */
#ifndef bool

#define false FALSE
#define true TRUE

typedef enum {
    false = 0,
    true = 1
} bool;

#endif

#endif /* __CKASTAL_CORE_INIT_C__ */