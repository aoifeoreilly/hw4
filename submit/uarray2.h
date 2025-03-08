/**************************************************************
 *
 *                     uarray2.h
 *
 *     Assignment: iii
 *     Authors:    Robert Stark (rstark03), Brendan Graziani (bgrazi02)
 *     Date:       2/10/2025
 *
 *     This file contains the interface for a 2D unboxed array.
 *
 **************************************************************/

#ifndef UARRAY2_H
#define UARRAY2_H

typedef struct UArray2_T *UArray2_T;

UArray2_T UArray2_new(int width, int height, int size);

void UArray2_free(UArray2_T *uarray2);

void *UArray2_at(UArray2_T uarray2, int col, int row);

int UArray2_width(UArray2_T uarray2);

int UArray2_height(UArray2_T uarray2);

int UArray2_size(UArray2_T uarray2);

void UArray2_map_row_major(UArray2_T uarray2,
                           void apply(int col,
                                      int row,
                                      UArray2_T uarray2,
                                      void *elem,
                                      void *cl),
                           void *cl);

void UArray2_map_col_major(UArray2_T uarray2,
                           void apply(int col,
                                      int row,
                                      UArray2_T uarray2,
                                      void *elem,
                                      void *cl),
                           void *cl);

#undef UArray2_T
#endif