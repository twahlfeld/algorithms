/*
 * =====================================================================================
 *
 *       Filename:  sort.c
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  09/13/2015 02:11:56
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Theodore Ahlfeld (), twahlfeld@gmail.com
 *   Organization:
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "sort.h"

Arr_Cont *copy_cont(Arr_Cont *src, int begin, size_t nel);
void *merge_sort_r(void *data);
void merge(Arr_Cont *base, Arr_Cont *L, Arr_Cont *R);
void free_arr_cont(Arr_Cont *data);
void check_pthread(int rc);

void check_pthread(int rc)
{
	if(rc) {
		fprintf(stderr, "ERROR; return code from pthread_create() is %d\n", rc);
		exit(-1);
	}
}

void cpy_elm(void *dst, void *src, int dst_off, int src_off, int width) {
	memcpy(dst+dst_off*width, src+src_off*width, width);
}

Arr_Cont *copy_cont(Arr_Cont *src, int begin, size_t nel) {
	int width = src->width;
	Arr_Cont *dst = (Arr_Cont *)malloc(sizeof(Arr_Cont));
	dst->base = (void *)malloc(width*nel);
	memcpy(dst->base, (src->base)+begin*width, width*nel);
	dst->nel = nel;
	dst->width = width;
	dst->cmp = src->cmp;
	return dst;
}

void merge_sort(void *base, size_t nel, size_t width,
				int (*cmp)(const void *, const void *))
{
	pthread_t main_thread;
	Arr_Cont sort_data;
	sort_data.base = base;
	sort_data.nel = nel;
	sort_data.width = width;
	sort_data.cmp = cmp;
	check_pthread(pthread_create(&main_thread, NULL, merge_sort_r, (void *) &sort_data));
	pthread_join(main_thread, NULL);
}

void *merge_sort_r(void *data)
{
	Arr_Cont *base = data;
	long nel = base->nel;
	if(nel > 1) {
		pthread_t pthread_L, pthread_R;
		int middle = nel/2;
		Arr_Cont *L = copy_cont(base, 0, middle);
		Arr_Cont *R = copy_cont(base, middle, nel-middle);
		check_pthread(pthread_create(&pthread_L, NULL, merge_sort_r, (void *)L));
		check_pthread(pthread_create(&pthread_R, NULL, merge_sort_r, (void *)R));
		pthread_join(pthread_L, NULL);
		pthread_join(pthread_R, NULL);
		merge(base, L, R);
		free_arr_cont(R);
		free_arr_cont(L);
	}
	pthread_exit(NULL);
}

void merge(Arr_Cont *base, Arr_Cont *L, Arr_Cont *R)
{
	int cur_L = 0, cur_R = 0, i;
	int width = base->width;
	for(i = 0; i < base->nel; i++) {
		if(cur_R == R->nel) {
			cpy_elm(base->base, L->base, i, cur_L++, width);
		} else if(cur_L == L->nel) {
			cpy_elm(base->base, R->base, i, cur_R++, width);
		} else {
			if(base->cmp(L->base+cur_L*width, R->base+cur_R*width) < 1) {
				cpy_elm(base->base, L->base, i, cur_L++, width);
			} else {
				cpy_elm(base->base, R->base, i, cur_R++, width);
			}
		}
	}
}

void free_arr_cont(Arr_Cont *data)
{
	free(data->base);
	free(data);
}

