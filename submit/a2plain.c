/**************************************************************
 *
 *                     a2plain.c
 *
 *     Assignment: locality
 *     Authors:    Robert Stark (rstark03), Aoife O'Reilly (aoreil02)
 *     Date:       2/20/2025
 *
 *     This file implements the UArray2 subclass of A2Methods_T.
 *     It provides methods to operate on a 2D unboxed array.
 *
 ************************/

#include <string.h>

#include <a2plain.h>
#include "uarray2.h"

// define a private version of each function in A2Methods_T that we implement

typedef A2Methods_UArray2 A2; // private abbreviation

/********** new ********
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
static A2Methods_UArray2 new(int width, int height, int size)
{
        return UArray2_new(width, height, size);
}

/********** new_with_blocksize ********
 *
 * Creates a new 2D unboxed array.
 *
 * Parameters:
 *      int width:      Number of columns in the 2D unboxed array.
 *      int height:     Number of rows in the 2D unboxed array.
 *      int size:       Size (in bytes) of each element in the 2D unboxed
 *                      array.
 *      int blocksize:  Counts the number of cells on one side of a block.
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
 *      Default blocksize size to 1.
 ************************/
static A2Methods_UArray2 new_with_blocksize(int width, int height, int size,
                                            int blocksize)
{
        (void)blocksize;
        return UArray2_new(width, height, size);
}

/********** a2free ********
 *
 * Frees a 2D unboxed array.
 *
 * Parameters:
 *      A2 *array2p:     Pointer to the 2D unboxed array to be freed.
 *
 * Return:
 *      Nothing.
 *
 * Expects:
 *      *array2p, array2p must not be NULL.
 *
 * Notes:
 *      Will CRE if any expectation is violated.
 *      Client is expected to free memory alloc'd for the elements stored.
 *      Sets array2p to NULL.
 ************************/
static void a2free(A2 *array2p)
{
        UArray2_free((UArray2_T *)array2p);
}

/********** width ********
 *
 * Gets the width (number of columns) of the 2D unboxed array.
 *
 * Parameters:
 *      A2 array2:      The 2D unboxed array to find the
 *                      width of.
 *
 * Return:
 *      An integer indicating the width.
 *
 * Expects:
 *      array2 must not be NULL.
 *
 * Notes:
 *      Will CRE if any expectation is violated.
 ************************/
static int width(A2 array2)
{
        return UArray2_width(array2);
}

/********** height ********
 *
 * Gets the height (number of rows) of the 2D unboxed array.
 *
 * Parameters:
 *      A2 array2:      The 2D unboxed array to find the
 *                      height of.
 *
 * Return:
 *      An integer indicating the height.
 *
 * Expects:
 *      array2 must not be NULL.
 *
 * Notes:
 *      Will CRE if any expectation is violated.
 ************************/
static int height(A2 array2)
{
        return UArray2_height(array2);
}

/********** size ********
 *
 * Gets the size (in bytes) of each element in the 2D unboxed array.
 *
 * Parameters:
 *      A2 array2:      The 2D unboxed array to find the size of.
 *
 * Return:
 *      An integer indicating the size (in bytes) of each element.
 *
 * Expects:
 *      array2 must not be NULL.
 *
 * Notes:
 *      Will CRE if any expectation is violated.
 ************************/
static int size(A2 array2)
{
        return UArray2_size(array2);
}

/********** blocksize ********
 *
 * Gets the size (in bytes) of each block in the 2D unboxed array.
 * This function will always return a value of 1.
 *
 * Parameters:
 *      A2 array2:      The 2D unboxed array to find the blocksize of.
 *
 * Return:
 *      An integer equal to 1.
 *
 * Expects:
 *      None.
 *
 * Notes:
 *      None.
 ************************/
static int blocksize(A2 array2)
{
        (void)array2;
        return 1;
}

/********** at ********
 *
 * Gets the element of the 2D unboxed array at the specified column and row.
 *
 * Parameters:
 *      A2 array2:     The 2D unboxed array where the element is located.
 *      int col:        Column index of the element.
 *      int row:        Row index of the element.
 *
 * Return:
 *      A void pointer to the element.
 *
 * Expects:
 *      array2 must not be NULL.
 *      col and row must be within valid range
 *      (0 <= col < width, 0 <= row < height).
 *
 * Notes:
 *      Will CRE if any expectation is violated.
 ************************/
static A2Methods_Object *at(A2 array2, int col, int row)
{
        return UArray2_at(array2, col, row);
}

/* apply function for map_row_major and map_col_major */
typedef void UArray2_applyfun(int col, 
                              int row, 
                              UArray2_T array2, 
                              void *elem, 
                              void *cl);


/********** map_row_major ********
 *
 * Applies the function specified in the second parameter to each element in
 * the 2D unboxed array. The function is applied by row then column.
 *
 * Parameters:
 *      A2 array2:                    The 2D unboxed array to be mapped.
 *      A2Methods_applyfun apply:     Function to be applied to each element.
 *      void *cl:                     Closure pointer.
 *
 * Return:
 *      Nothing.
 *
 * Expects:
 *      array2 must not be NULL.
 *      The apply function should accept parameters
 *      (int col, int row, UArray2_T array2, void *elem, void *cl).
 *      If used, cl can be NULL.
 *
 * Notes:
 *      Will CRE if any expectation is violated.
 *      The closure pointer (cl) can be used to pass extra data
 *      to the apply function.
 ************************/
static void map_row_major(A2 array2,
                          A2Methods_applyfun apply,
                          void *cl)
{
        UArray2_map_row_major(array2, (UArray2_applyfun *) apply, cl);
}

/********** map_col_major ********
 *
 * Applies the function specified in the second parameter to each element in
 * the 2D unboxed array. The function is applied by column then row.
 *
 * Parameters:
 *      A2 array2:                    The 2D unboxed array to be mapped.
 *      A2Methods_applyfun apply:     Function to be applied to each element.
 *      void *cl:                     Closure pointer.
 *
 * Return:
 *      Nothing.
 *
 * Expects:
 *      array2 must not be NULL.
 *      The apply function should accept parameters
 *      (int col, int row, UArray2_T array2, void *elem, void *cl).
 *      If used, cl can be NULL.
 *
 * Notes:
 *      Will CRE if any expectation is violated.
 *      The closure pointer (cl) can be used to pass extra data
 *      to the apply function.
 ************************/
static void map_col_major(A2 array2,
                          A2Methods_applyfun apply,
                          void *cl)
{
        UArray2_map_col_major(array2, (UArray2_applyfun *)apply, cl);
}

struct small_closure
{
        A2Methods_smallapplyfun *apply;
        void *cl;
};

/* apply function for small_map_row_major and small_map_col_major */
static void apply_small(int col, 
                        int row, 
                        UArray2_T array2p,
                        void *elem, 
                        void *vcl)
{
        struct small_closure *cl = vcl;
        (void)col;
        (void)row;
        (void)array2p;
        cl->apply(elem, cl->cl);
}

/********** small_map_row_major ********
 *
 * Applies the function specified in the second parameter to each element in
 * the 2D unboxed array. The function is applied by row then column.
 * 
 * This function is used for mappings where the index of the element is not
 * needed.
 *
 * Parameters:
 *      A2 a2:                             The 2D unboxed array to be mapped.
 *      A2Methods_smallapplyfun apply:     Function to be applied to each 
 *                                         element.
 *      void *cl:                          Closure pointer.
 *
 * Return:
 *      Nothing.
 *
 * Expects:
 *      array2 must not be NULL.
 *      The apply function should accept parameters
 *      (int col, int row, A2 array2, void *elem, void *cl).
 *      If used, cl can be NULL.
 *
 * Notes:
 *      Will CRE if any expectation is violated.
 *      The closure pointer (cl) can be used to pass extra data
 *      to the apply function.
 ************************/
static void small_map_row_major(A2 a2,
                                A2Methods_smallapplyfun apply,
                                void *cl)
{
        struct small_closure mycl = {apply, cl};
        UArray2_map_row_major(a2, apply_small, &mycl);
}

/********** small_map_col_major ********
 *
 * Applies the function specified in the second parameter to each element in
 * the 2D unboxed array. The function is applied by column then row.
 * 
 * This function is used for mappings where the index of the element is not
 * needed.
 *
 * Parameters:
 *      A2 a2:                             The 2D unboxed array to be mapped.
 *      A2Methods_smallapplyfun apply:     Function to be applied to each 
 *                                         element.
 *      void *cl:                          Closure pointer.
 *
 * Return:
 *      Nothing.
 *
 * Expects:
 *      a2 must not be NULL.
 *      The apply function should accept parameters
 *      (int col, int row, A2 array2p, void *elem, void *cl).
 *      If used, cl can be NULL.
 *
 * Notes:
 *      Will CRE if any expectation is violated.
 *      The closure pointer (cl) can be used to pass extra data
 *      to the apply function.
 ************************/
static void small_map_col_major(A2 a2,
                                A2Methods_smallapplyfun apply,
                                void *cl)
{
        struct small_closure mycl = {apply, cl};
        UArray2_map_col_major(a2, apply_small, &mycl);
}

/********** A2Methods_T ********
 *
 * This structure represents a set of methods for manipulating 2D 
 * unboxed arrays.
 *
 * Methods:
 *      new:                   
 *      new_with_blocksize:    
 *      a2free:                
 *      width:                 
 *      height:               
 *      size:                 
 *      blocksize:            
 *      at:                    
 *      map_row_major:         
 *      map_col_major:         
 *      map_block_major:       Not implemented.
 *      map_default:           
 *      small_map_row_major:   
 *      small_map_col_major:   
 *      small_map_block_major: Not implemented.
 *      small_map_default:     
 ************************/
static struct A2Methods_T uarray2_methods_plain_struct = {
    new,
    new_with_blocksize,
    a2free,
    width,
    height,
    size,
    blocksize, // blocksize
    at,
    map_row_major,
    map_col_major,
    NULL,          // map_block_major
    map_row_major, // map_default
    small_map_row_major,
    small_map_col_major,
    NULL,                // small_map_block_major
    small_map_row_major, // small_map_default
};

// finally the payoff: here is the exported pointer to the struct

A2Methods_T uarray2_methods_plain = &uarray2_methods_plain_struct;