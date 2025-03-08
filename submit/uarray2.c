/**************************************************************
 *
 *                     uarray2.c
 *
 *     Assignment: iii
 *     Authors:  Robert Stark (rstark03), Brendan Graziani (bgrazi02)
 *     Date:     2/10/2025
 *
 *     This file contains the implementation for a 2D unboxed array as
 *     declared in uarray2.h.
 *
 **************************************************************/

#include "uarray2.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <uarray.h>

/********** UArray2_T ********
 *
 * This structure represents a 2D unboxed array.
 * It uses a 1D unboxed array (UArray_T) to store the elements.
 *
 * Elements:
 *      int width:           The width of the 2D unboxed array.
 *      int height:          The height of the 2D unboxed array.
 *      UArray2_T array:     The underlying 1D unboxed array that stores the
 *                           elements.
 ************************/
struct UArray2_T 
{
        int width;
        int height;
        UArray_T array;
};

/********** UArray2_new ********
 *
 * Creates a new 2D unboxed array.
 *
 * Parameters:
 *      int width:      Number of columns in the 2D unboxed array.
 *      int height:     Number of rows in the 2D unboxed array.
 *      int size:       Size (in bytes) of each element in the 2D unboxed
 *                      array.
 *
 * Return:
 *      A new 2D unboxed array.
 *
 * Expects:
 *      width, hieght size >= 0.
 *
 * Notes:
 *      Will CRE if memory allocation fails.
 *      Will CRE if any expectation is violated.
 *      Client is expected to free the array using the UArray2_free method.
 ************************/
UArray2_T UArray2_new(int width, int height, int size)
{
        UArray2_T uarray2 = malloc(sizeof(*uarray2));
        assert(uarray2 != NULL);
        uarray2->width = width;
        uarray2->height = height;
        uarray2->array = UArray_new(width * height, size);
        return uarray2;
}

/********** UArray2_free ********
 *
 * Frees a 2D unboxed array.
 *
 * Parameters:
 *      UArray2_T *uarray2:     Pointer to the 2D unboxed array to be freed.
 *
 * Return:
 *      Nothing.
 *
 * Expects:
 *      *uarray2, uarray2 must not be NULL.
 *
 * Notes:
 *      Will CRE if any expectation is violated.
 *      Client is expected to free memory alloc'd for the elements stored.
 *      Sets uarray2 to NULL.
 ************************/
void UArray2_free(UArray2_T *uarray2)
{
        assert((uarray2 != NULL) && (*uarray2 != NULL));
        UArray_free(&((*uarray2)->array));
        free(*uarray2);
        *uarray2 = NULL;
}

/********** UArray2_at ********
 *
 * Gets the element of the 2D unboxed array at the specified column and row.
 *
 * Parameters:
 *      UArray2_T uarray2:      The 2D unboxed array where the element is
 *                              located.
 *      int col:                Column index of the element.
 *      int row:                Row index of the element.
 *
 * Return:
 *      A void pointer to the element.
 *
 * Expects:
 *      uarray2 must not be NULL.
 *      col and row must be within valid range
 *      (0 <= col < width, 0 <= row < height).
 *
 * Notes:
 *      Will CRE if any expectation is violated.
 ************************/
void *UArray2_at(UArray2_T uarray2, int col, int row)
{
        assert(uarray2 != NULL);
        assert(col >= 0 && col < uarray2->width);
        assert(row >= 0 && row < uarray2->height);
        return UArray_at(uarray2->array, col + (row * uarray2->width));
}

/********** UArray2_width ********
 *
 * Gets the width (number of columns) of the 2D unboxed array.
 *
 * Parameters:
 *      UArray2_T uarray2:      The 2D unboxed array to find the width of.
 *
 * Return:
 *      An integer indicating the width.
 *
 * Expects:
 *      uarray2 must not be NULL.
 *
 * Notes:
 *      Will CRE if any expectation is violated.
 ************************/
int UArray2_width(UArray2_T uarray2)
{
        assert(uarray2 != NULL);
        return uarray2->width;
}

/********** UArray2_height ********
 *
 * Gets the height (number of rows) of the 2D unboxed array.
 *
 * Parameters:
 *      UArray2_T uarray2:      The 2D unboxed array to find the height of.
 *
 * Return:
 *      An integer indicating the height.
 *
 * Expects:
 *      uarray2 must not be NULL.
 *
 * Notes:
 *      Will CRE if any expectation is violated.
 ************************/
int UArray2_height(UArray2_T uarray2)
{
        assert(uarray2 != NULL);
        return uarray2->height;
}

/********** UArray2_size ********
 *
 * Gets the size (in bytes) of each element in the 2D unboxed array.
 *
 * Parameters:
 *      UArray2_T uarray2:      The 2D unboxed array to find the size of.
 *
 * Return:
 *      An integer indicating the size (in bytes) of each element.
 *
 * Expects:
 *      uarray2 must not be NULL.
 *
 * Notes:
 *      Will CRE if any expectation is violated.
 ************************/
int UArray2_size(UArray2_T uarray2)
{
        assert(uarray2 != NULL);
        return UArray_size(uarray2->array);
}

/********** UArray2_map_row_major ********
 *
 * Applies the function specified in the second parameter to each element in
 * the 2D unboxed array. The function is applied by row then column.
 *
 * Parameters:
 *      UArray2_T uarray2:      The 2D unboxed array to be mapped.
 *      void apply:             Function to be applied to each element.
 *      void *cl:               Closure pointer.
 *
 * Return:
 *      Nothing.
 *
 * Expects:
 *      uarray2 must not be NULL.
 *      The apply function should accept parameters
 *      (int col, int row, UArray2_T uarray2, void *elem, void *cl).
 *      If used, cl can be NULL.
 *
 * Notes:
 *      Will CRE if any expectation is violated.
 *      The closure pointer (cl) can be used to pass extra data
 *      to the apply function.
 ************************/
void UArray2_map_row_major(UArray2_T uarray2,
                           void apply(int col,
                                      int row,
                                      UArray2_T uarray2,
                                      void *elem,
                                      void *cl),
                           void *cl)
{
        assert(uarray2 != NULL);
        assert(apply != NULL);
        int height = uarray2->height;
        int width = uarray2->width;
        for (int row = 0; row < height; row++) {
                for (int col = 0; col < width; col++) {
                        void *elem = UArray2_at(uarray2, col, row);
                        apply(col, row, uarray2, elem, cl);
                }
        }
}

/********** UArray2_map_col_major ********
 *
 * Applies the function specified in the second parameter to each element in
 * the 2D unboxed array. The function is applied by column then row.
 *
 * Parameters:
 *      UArray2_T uarray2:      The 2D unboxed array to be mapped.
 *      void apply:             Function to be applied to each element.
 *      void *cl:               Closure pointer.
 *
 * Return:
 *      Nothing.
 *
 * Expects:
 *      uarray2 must not be NULL.
 *      The apply function should accept parameters
 *      (int col, int row, UArray2_T uarray2, void *elem, void *cl)
 *      and return void.
 *      If used, cl can be NULL.
 *
 * Notes:
 *      Will CRE if any expectation is violated.
 *      The closure pointer (cl) can be used to pass extra data
 *      to the apply function.
 ************************/
void UArray2_map_col_major(UArray2_T uarray2,
                           void apply(int col,
                                      int row,
                                      UArray2_T uarray2,
                                      void *elem,
                                      void *cl),
                           void *cl)
{
        assert(uarray2 != NULL);
        assert(apply != NULL);
        int height = uarray2->height;
        int width = uarray2->width;
        for (int col = 0; col < width; col++) {
                for (int row = 0; row < height; row++) {
                        void *elem = UArray2_at(uarray2, col, row);
                        apply(col, row, uarray2, elem, cl);
                }
        }
}