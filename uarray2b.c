/**************************************************************
 *
 *                     uarray2b.c
 *
 *     Assignment: locality
 *     Authors:    Robert Stark (rstark03), Aoife O'Reilly (aoreil02)
 *     Date:       2/20/2025
 *
 *     This file contains the implementation for a 2D unboxed, blocked array.
 *
 **************************************************************/

#include "uarray2b.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <uarray.h>

/* 64KB = sqrt(64 * 1024) = 256 */
#define SIZE_64KB 65536
#define BLOCKSIZE_64KB 256

/********** UArray2b_T ********
 *
 * Struct for the 2D unboxed, blocked array where elements are stored in 
 * blocks of size blocksize * blocksize.
 * It uses a 1D unboxed array (UArray_T) to store the elements.
 *
 * Elements:
 *      int width:      Number of columns in the 2D unboxed, blocked array.
 *      int height:     Number of rows in the 2D unboxed, blocked array.
 *      int size:       Size (in bytes) of each element.
 *      int blocksize:  Number of cells on one side of a block.
 *      int b_cols:     Number of columns of blocks.
 *      int b_rows:     Number of rows of blocks.
 *      UArray_T array: The underlying 1D unboxed array that stores the
 *                      elements.
 ************************/
struct UArray2b_T
{
        int width;
        int height;
        int size;
        int blocksize;
        int b_cols;
        int b_rows;
        UArray_T array;
};

/********** UArray2b_new ********
 *
 * Creates a new 2D unboxed, blocked array.
 *
 * Parameters:
 *      int width:      Number of columns in the 2D unboxed, blocked array.
 *      int height:     Number of rows in the 2D unboxed, blocked array.
 *      int size:       Size (in bytes) of each element.
 *      int blocksize:  Counts the number of cells on one side of a block.
 *
 * Return:
 *      A new 2D unboxed, blocked array.
 *
 * Expects:
 *      width, height, size, blocksize >= 0.
 *
 * Notes:
 *      Will CRE if memory allocation fails.
 *      Will CRE if any expectation is violated.
 *      Client is expected to free the array using the UArray2_free method.
 ************************/
UArray2b_T UArray2b_new(int width, int height, int size, int blocksize)
{
        /* Allocate memory for new UArray2b_T structure */
        UArray2b_T array2b = malloc(sizeof(*array2b));
        assert(array2b != NULL);

        assert(blocksize > 0);
        array2b->width = width;
        array2b->height = height;
        array2b->size = size;
        array2b->blocksize = blocksize;

        /* Compute number of blocks and adjust if not a clean fit */
        array2b->b_cols = width / blocksize;
        if (width % blocksize != 0) {
                array2b->b_cols++;
        }
        array2b->b_rows = height / blocksize;
        if (height % blocksize != 0) {
                array2b->b_rows++;
        }

        int mem_width = array2b->b_cols * blocksize;
        int mem_height = array2b->b_rows * blocksize;
        int total_memory = mem_width * mem_height;
        array2b->array = UArray_new(total_memory, size);

        return array2b;
}

/********** UArray2b_new_64K_block ********
 *
 * Creates a new 2D unboxed, blocked array with maximum block size (64K).
 *
 * Parameters:
 *      int width:      Number of columns in the 2D unboxed, blocked array.
 *      int height:     Number of rows in the 2D unboxed, blocked array.
 *      int size:       Size (in bytes) of each element.
 *
 * Return:
 *      A new 2D unboxed, blocked array.
 *
 * Expects:
 *      width, height, size >= 0.
 *
 * Notes:
 *      If size > 64K, then blocksize will default to 1.
 *      Will CRE if memory allocation fails.
 *      Will CRE if any expectation is violated.
 *      Client is expected to free the array using the UArray2_free method.
 ************************/
UArray2b_T UArray2b_new_64K_block(int width, int height, int size)
{
        int blocksize = (size > SIZE_64KB) ? 1 : BLOCKSIZE_64KB;
        return UArray2b_new(width, height, size, blocksize);
}

/********** UArray2b_free ********
 *
 * Frees a 2D unboxed, blocked array.
 *
 * Parameters:
 *      UArray2b_T *array2b:     Pointer to the 2D unboxed, blocked array to
 *                               be freed.
 *
 * Return:
 *      Nothing.
 *
 * Expects:
 *      *array2b, array2b must not be NULL.
 *
 * Notes:
 *      Will CRE if any expectation is violated.
 *      Client is expected to free memory alloc'd for the elements stored.
 *      Sets array2b to NULL.
 ************************/
void UArray2b_free(UArray2b_T *array2b)
{
        assert((array2b != NULL) && (*array2b != NULL));
        UArray_free(&((*array2b)->array));
        free(*array2b);
        *array2b = NULL;
}

/********** UArray2b_width ********
 *
 * Gets the width (number of columns) of the 2D unboxed, blocked array.
 *
 * Parameters:
 *      UArray2b_T array2b:      The 2D unboxed, blocked array to find the
 *                               width of.
 *
 * Return:
 *      An integer indicating the width.
 *
 * Expects:
 *      array2b must not be NULL.
 *
 * Notes:
 *      Will CRE if any expectation is violated.
 ************************/
int UArray2b_width(UArray2b_T array2b)
{
        assert(array2b != NULL);
        return array2b->width;
}

/********** UArray2b_height ********
 *
 * Gets the height (number of rows) of the 2D unboxed, blocked array.
 *
 * Parameters:
 *      UArray2b_T array2b:      The 2D unboxed, blocked array to find the
 *                               height of.
 *
 * Return:
 *      An integer indicating the height.
 *
 * Expects:
 *      array2b must not be NULL.
 *
 * Notes:
 *      Will CRE if any expectation is violated.
 ************************/
int UArray2b_height(UArray2b_T array2b)
{
        assert(array2b != NULL);
        return array2b->height;
}

/********** UArray2b_size ********
 *
 * Gets the size (in bytes) of each element in the 2D unboxed, blocked array.
 *
 * Parameters:
 *      UArray2b_T array2b:      The 2D unboxed, blocked array to find the size
 *                               of.
 *
 * Return:
 *      An integer indicating the size (in bytes) of each element.
 *
 * Expects:
 *      array2b must not be NULL.
 *
 * Notes:
 *      Will CRE if any expectation is violated.
 ************************/
int UArray2b_size(UArray2b_T array2b)
{
        assert(array2b != NULL);
        return array2b->size;
}

/********** UArray2b_blocksize ********
 *
 * Gets the size (in bytes) of each block in the 2D unboxed, blocked array.
 * Block size counts the number of cells on one side of a block.
 *
 * Parameters:
 *      UArray2b_T array2b:      The 2D unboxed, blocked array to find the
 *                               blocksize of.
 *
 * Return:
 *      An integer indicating the size (in bytes) of each block.
 *
 * Expects:
 *      array2b must not be NULL.
 *
 * Notes:
 *      Will CRE if any expectation is violated.
 ************************/
int UArray2b_blocksize(UArray2b_T array2b)
{
        assert(array2b != NULL);
        return array2b->blocksize;
}


/********** getIndex ********
 *
 * Gets the index of the element in the underlying 1D array for the given
 * column and row in the 2D unboxed, blocked array.
 *
 * Parameters:
 *      UArray2b_T array2b:     The 2D unboxed, blocked array.
 *      int col:                Column index of the element.
 *      int row:                Row index of the element.
 *
 * Return:
 *      An integer indicating the index in the underlying 1D array.
 *
 * Expects:
 *      array2b must not be NULL.
 *      col and row must be within valid range
 *      (0 <= col < width, 0 <= row < height).
 *
 * Notes:
 *      Will CRE if any expectation is violated.
 *      This function is not available to the client.
 ************************/
static int getIndex(UArray2b_T array2b, int col, int row)
{
        assert(array2b != NULL);
        assert(col >= 0 && col < array2b->width);
        assert(row >= 0 && row < array2b->height);

        /* 
         * Get the variables to describe the position of the index in the
         * 2D unboxed, blocked array.
         * 
         * b_col:       The column of the block that contains the index.
         * b_row:       The row of the block that contains the index.
         * col_in_b:    The column within the block that contains the index.
         * row_in_b:    The row within the block that contains the index.
         */
        int blocksize = array2b->blocksize;
        int b_col = col / blocksize;
        int b_row = row / blocksize;
        int col_in_b = col % blocksize;
        int row_in_b = row % blocksize;

        /* Move to the starting index of the block that contains the index */
        int curr_b = (b_row * array2b->b_cols * blocksize * blocksize) +
                     (b_col * blocksize * blocksize);
        /* Calculate the index within the block */
        int index = curr_b + (row_in_b * blocksize) + col_in_b;

        return index;
}

/********** UArray2b_at ********
 *
 * Gets the element of the 2D unboxed, blocked array at the specified column
 * and row.
 *
 * Parameters:
 *      UArray2b_T array2b:     The 2D unboxed, blocked array where the element
 *                              is located.
 *      int col:                Column index of the element.
 *      int row:                Row index of the element.
 *
 * Return:
 *      A void pointer to the element.
 *
 * Expects:
 *      array2b must not be NULL.
 *      col and row must be within valid range
 *      (0 <= col < width, 0 <= row < height).
 *
 * Notes:
 *      Will CRE if any expectation is violated.
 ************************/
void *UArray2b_at(UArray2b_T array2b, int col, int row)
{
        assert(array2b != NULL);
        assert(col >= 0 && col < array2b->width);
        assert(row >= 0 && row < array2b->height);

        int index = getIndex(array2b, col, row);
        return UArray_at(array2b->array, index);
}

/********** UArray2b_map ********
 *
 * Applies the function specified in the second parameter to each element in
 * the 2D unboxed, blocked array. The function is applied by block, and
 * row-major within each block.
 *
 * Parameters:
 *      UArray2b_T array2b:     The 2D unboxed, blocked array to be mapped.
 *      void apply:             Function to be applied to each element.
 *      void *cl:               Closure pointer.
 *
 * Return:
 *      Nothing.
 *
 * Expects:
 *      array2b, apply must not be NULL.
 *      The apply function should accept parameters
 *      (int col, int row, UArrayb2_T array2b, void *elem, void *cl).
 *      If used, cl can be NULL.
 *
 * Notes:
 *      Will CRE if any expectation is violated.
 *      The closure pointer (cl) can be used to pass extra data
 *      to the apply function.
 ************************/
void UArray2b_map(UArray2b_T array2b,
                  void apply(int x,
                             int y,
                             UArray2b_T array2b,
                             void *elem,
                             void *cl),
                  void *cl)
{
        assert(array2b != NULL);
        assert(apply != NULL);

        int blocksize = array2b->blocksize;
        int b_rows = array2b->b_rows;
        int b_cols = array2b->b_cols;
        int height = array2b->height;
        int width = array2b->width;

        /* Iterate over each block in the array */
        for (int b_row = 0; b_row < b_rows; b_row++) {
                for (int b_col = 0; b_col < b_cols; b_col++) {

                        /* Get the starting index of the block we are in */
                        /* Not using getIndex to avoid repetative arith */
                        int curr_b = (b_row * b_cols * blocksize * blocksize) +
                                     (b_col * blocksize * blocksize);

                        /* For each block iterate by relative row-major */
                        for (int row_in_b = 0; row_in_b < blocksize; 
                             row_in_b++) {

                                int row = b_row * blocksize + row_in_b;
                                if (row >= height) {
                                        continue;
                                }

                                for (int col_in_b = 0; col_in_b < blocksize;
                                     col_in_b++) {
          
                                        int col = b_col * blocksize + col_in_b;
                                        if (col >= width) {
                                                continue;
                                        }

                                        /* 
                                         * Calculate the index of the elem 
                                         * w.r.t. the block 
                                         */
                                        int index = curr_b +
                                                    (row_in_b * blocksize) +
                                                    col_in_b;

                                        void *elem = UArray_at(array2b->array,
                                                               index);
                                                               
                                        apply(col, row, array2b, elem, cl);
                                }
                        }
                }
        }
}
