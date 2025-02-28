#ifndef __CKASTAL_PROVIDER_LIST_SINGLE_FUNCTION_BUBBLESORT_H__
#define __CKASTAL_PROVIDER_LIST_SINGLE_FUNCTION_BUBBLESORT_H__

#include "../core/main.c"

#define CK_LIST_INLINE_BUBBLESORT_FN(list, type, block) \
    {                                                   \
        if (list.length > 1) {                          \
            int swapped;                                \
            Ck_ListNode* ptr1;                          \
            Ck_ListNode* lptr = NULL;                   \
            do {                                        \
                swapped = 0;                            \
                ptr1 = list.head;                       \
                while (ptr1->next != lptr) {            \
                    type a = (type)ptr1->data;          \
                    type b = (type)ptr1->next->data;    \
                    int order;                          \
                    {                                   \
                        block                           \
                    }                                   \
                    if (order > 0) {                    \
                        void* temp = ptr1->data;        \
                        ptr1->data = ptr1->next->data;  \
                        ptr1->next->data = temp;        \
                        swapped = 1;                    \
                    }                                   \
                    ptr1 = ptr1->next;                  \
                }                                       \
                lptr = ptr1;                            \
            } while (swapped);                          \
        }                                               \
    }

#endif /** __CKASTAL_PROVIDER_LIST_SINGLE_FUNCTION_BUBBLESORT_H__ */
