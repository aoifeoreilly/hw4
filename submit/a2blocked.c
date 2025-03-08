#include <string.h>

#include <a2blocked.h>
#include "uarray2b.h"

// define a private version of each function in A2Methods_T that we implement

typedef A2Methods_UArray2 A2;   // private abbreviation


/********** new ********
 *
 * Creates a new 2D unboxed, blocked array.
 *
 * Parameters:
 *      int width:      Number of columns in the 2D unboxed, blocked array.
 *      int height:     Number of rows in the 2D unboxed, blocked array.
 *      int size:       Size (in bytes) of each element in the 2D unboxed, 
 *                      blocked array.
 *
 * Return:
 *      A new 2D unboxed, blocked array.
 *
 * Expects:
 *      width, hieght size >= 0.
 *
 * Notes:
 *      Will CRE if memory allocation fails.
 *      Will CRE if any expectation is violated.
 *      Client is expected to free the array using the UArray2b_free method.
 *      Defaults blocksize to 256 (64KB).
 ************************/
static A2 new(int width, int height, int size)
{
        return UArray2b_new_64K_block(width, height, size);
}

/********** new_with_blocksize ********
 *
 * Creates a new 2D unboxed, blocked array.
 *
 * Parameters:
 *      int width:      Number of columns in the 2D unboxed, blocked array.
 *      int height:     Number of rows in the 2D unboxed, blocked array.
 *      int size:       Size (in bytes) of each element in the 2D unboxed, 
 *                      blocked array.
 *      int blocksize:  Counts the number of cells on one side of a block.
 *
 * Return:
 *      A new 2D unboxed, blocked array.
 *
 * Expects:
 *      width, hieght size >= 0.
 *
 * Notes:
 *      Will CRE if memory allocation fails.
 *      Will CRE if any expectation is violated.
 *      Client is expected to free the array using the UArray2b_free method.
 ************************/
static A2 new_with_blocksize(int width, int height, int size, int blocksize)
{
        return UArray2b_new(width, height, size, blocksize);
}

/********** a2free ********
 *
 * Frees a 2D unboxed, blocked array.
 *
 * Parameters:
 *      A2 *array2p:     Pointer to the 2D unboxed, blocked array to be freed.
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
static void a2free(A2 * array2p)
{
        UArray2b_free((UArray2b_T *) array2p);
}

/********** width ********
 *
 * Gets the width (number of columns) of the 2D unboxed, blocked array.
 *
 * Parameters:
 *      A2 array2:      The 2D unboxed, blocked array to find the
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
        return UArray2b_width(array2);
}

/********** height ********
 *
 * Gets the height (number of rows) of the 2D unboxed, blocked array.
 *
 * Parameters:
 *      A2 array2:      The 2D unboxed, blocked array to find the
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
        return UArray2b_height(array2);
}

/********** size ********
 *
 * Gets the size (in bytes) of each element in the 2D unboxed, blocked array.
 *
 * Parameters:
 *      A2 array2:      The 2D unboxed, blocked array to find the size of.
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
        return UArray2b_size(array2);
}

/********** blocksize ********
 *
 * Gets the size (in bytes) of each block in the 2D unboxed, blocked array.
 * Block size counts the number of cells on one side of a block.
 *
 * Parameters:
 *      A2 array2:      The 2D unboxed, blocked array to find the
 *                      blocksize of.
 *
 * Return:
 *      An integer indicating the size (in bytes) of each block.
 *
 * Expects:
 *      array2 must not be NULL.
 *
 * Notes:
 *      Will CRE if any expectation is violated.
 ************************/
static int blocksize(A2 array2)
{
        return UArray2b_blocksize(array2);
}

/********** at ********
 *
 * Gets the element of the 2D unboxed, blocked array at the specified 
 * column and row.
 *
 * Parameters:
 *      A2 array2:     The 2D unboxed, blocked array where the element is 
 *                     located.
 *      int col:       Column index of the element.
 *      int row:       Row index of the element.
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
        return UArray2b_at(array2, col, row);
}

/* apply function for map_block_major */
typedef void applyfun(int col, int row, UArray2b_T array2b, void *elem, 
                      void *cl);

/********** map_block_major ********
 *
 * Applies the function specified in the second parameter to each element in
 * the 2D unboxed, blocked array. The function is applied by block, and
 * row-major within each block.
 *
 * Parameters:
 *      A2:                           The 2D unboxed, blocked array to be 
 *                                    mapped.
 *      A2Methods_applyfun apply:     Function to be applied to each element.
 *      void *cl:                     Closure pointer.
 *
 * Return:
 *      Nothing.
 *
 * Expects:
 *      array2p must not be NULL.
 *      The apply function should accept parameters
 *      (int col, int row, UArray2b_T array2p, void *elem, void *cl).
 *      If used, cl can be NULL.
 *
 * Notes:
 *      Will CRE if any expectation is violated.
 *      The closure pointer (cl) can be used to pass extra data
 *      to the apply function.
 ************************/
static void map_block_major(A2 array2, A2Methods_applyfun apply, void *cl)
{
        UArray2b_map(array2, (applyfun *) apply, cl);
}

struct small_closure {
        A2Methods_smallapplyfun *apply;
        void *cl;
};

/* apply function for small_map_block_major */
static void apply_small(int col, 
                        int row, 
                        UArray2b_T array2, 
                        void *elem, 
                        void *vcl)
{
        struct small_closure *cl = vcl;
        (void)col;
        (void)row;
        (void)array2;
        cl->apply(elem, cl->cl);
}

/********** small_map_block_major ********
 *
 * Applies the function specified in the second parameter to each element in
 * the 2D unboxed, blocked array. The function is applied by block, and
 * row-major within each block.
 * 
 * This function is used for mappings where the index of the element is not
 * needed.
 *
 * Parameters:
 *      A2 a2:                             The 2D unboxed, blocked array to be 
 *                                         mapped.
 *      A2Methods_smallapplyfun apply:     Function to be applied to each 
 *                                         element.
 *      void *cl:                          Closure pointer.
 *
 * Return:
 *      Nothing.
 *
 * Expects:
 *      array2p must not be NULL.
 *      The apply function should accept parameters
 *      (int col, int row, UArray2b_T array2p, void *elem, void *cl).
 *      If used, cl can be NULL.
 *
 * Notes:
 *      Will CRE if any expectation is violated.
 *      The closure pointer (cl) can be used to pass extra data
 *      to the apply function.
 ************************/
static void small_map_block_major(A2 a2, 
                                  A2Methods_smallapplyfun apply,
                                  void *cl)
{
        struct small_closure mycl = { apply, cl };
        UArray2b_map(a2, apply_small, &mycl);
}

/********** A2Methods_T ********
 *
 * This structure represents a set of methods for manipulating 2D 
 * unboxed, blocked arrays.
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
 *      map_row_major:         Not implemented.
 *      map_col_major:         Not implemented.
 *      map_block_major:       
 *      map_default:           
 *      small_map_row_major:   Not implemented.
 *      small_map_col_major:   Not implemented.
 *      small_map_block_major:
 *      small_map_default:     
 ************************/
static struct A2Methods_T uarray2_methods_blocked_struct = {
        new,
        new_with_blocksize,
        a2free,
        width,
        height,
        size,
        blocksize,
        at,
        NULL,                   // map_row_major
        NULL,                   // map_col_major
        map_block_major,
        map_block_major,        // map_default
        NULL,                   // small_map_row_major
        NULL,                   // small_map_col_major
        small_map_block_major,
        small_map_block_major,  // small_map_default
};

// finally the payoff: here is the exported pointer to the struct

A2Methods_T uarray2_methods_blocked = &uarray2_methods_blocked_struct;
