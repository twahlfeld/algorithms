/*
 * =====================================================================================
 *
 *       Filename:  sort.h
 *
 *    Description:  Multi-Threaded Sort Algorithms
 *
 *        Version:  1.0
 *        Created:  09/13/2015 02:02:49
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Theodore Ahlfeld (), twahlfeld@gmail.com
 *   Organization:
 *
 * =====================================================================================
 */

#ifndef __PTHRD_SORT__
#define __PTHRD_SORT__

typedef struct Arr_Cont {
    void *base;
    size_t nel;
    size_t width;
    int (*cmp)(const void *, const void *);
} Arr_Cont;

void merge_sort(void *base, size_t nel, size_t width,
                int (*cmp)(const void *, const void *));

#endif
