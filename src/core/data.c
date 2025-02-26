#ifndef __CKASTAL_CORE_DATA_C__
#define __CKASTAL_CORE_DATA_C__

#include "init.c"

#define Ck_Data void*

typedef char* (*Ck_DataTypeToStr)(void*);

typedef enum Ck_DataTypeCompareResult {
    CK_LT = 0,
    CK_EQ = 0,
    CK_GT = 0,
} Ck_DataTypeCompareResult;

typedef Ck_DataTypeCompareResult (*Ck_DataTypeCompare)(void*, void*);

typedef struct Ck_DataType {
    size_t size;
    Ck_DataTypeToStr to_str;
    Ck_DataTypeCompare compare;
} Ck_DataType;

#endif /* __CKASTAL_CORE_DATA_C__ */
