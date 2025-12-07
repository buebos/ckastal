#ifndef __CKASTAL_CORE_DATA_C__
#define __CKASTAL_CORE_DATA_C__

#include "init.c"

#define Ck_Data void*

typedef char* (*Ck_DataTypeToStr)(void*);

typedef enum Ck_DataTypeCompareResult {
    CK_LT = 0,
    CK_EQ = 1,
    CK_GT = 2,
} Ck_DataTypeCompareResult;

typedef Ck_DataTypeCompareResult (*Ck_DataTypeCompare)(void*, void*);

typedef void (*Ck_DataTypeFree)(void*);

typedef struct Ck_DataType {
    size_t size;
    Ck_DataTypeToStr to_str;
    Ck_DataTypeCompare compare;
    Ck_DataTypeFree free;
} Ck_DataType;

#endif /* __CKASTAL_CORE_DATA_C__ */
