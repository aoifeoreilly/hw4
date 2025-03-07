/**************************************************************
 *
 *                     pack_unpack.c
 *
 *      Assignment: arith
 *      Authors:    Griffin Faecher (gfaech01) and Aoife O'Reilly (aoreil02)
 *      Date:       3/5/2025
 *
 *      This file packs and unpacks a, b, c, d, PB, and PR between local 
 *      variables and 32-bit code words.
 *
 *************************************************************/

#include "pack_unpack.h"

/********** pack_image ********
 *
 *  Converts the local variables a, b, c, d, Pb_avg, and Pr_avg to 32-bit 
 *  code words using the Bitpack interface.
 *
 * Parameters:
 *      UArray2b_T DCT_image: A 2D blocked array of local variables
 *                            a, b, c, d, Pb_avg, and Pr_avg.
 * 
 * Return: 
 *      A 2D array of 32-bit code words.
 *
 * Expects:
 *     A valid 2D blocked array.
 * 
 * Notes:
 *      Allocates a new 2D array to store the 32-bit codewords.
 *      Will C.R.E. if the 2D blocked array is not valid.
 *      Will C.R.E. if the 2D array is not allocated correctly.
 * 
 ************************/
UArray2_T pack_image(UArray2b_T DCT_image)
{
        assert(DCT_image != NULL);
        UArray2_T packed_image = UArray2_new(UArray2b_width(DCT_image),
                                                UArray2b_height(DCT_image), 
                                                sizeof(uint32_t));
        assert(packed_image != NULL);
        UArray2b_map(DCT_image, pack_image_apply, packed_image);
        
        return packed_image;
}

/********** pack_image_apply ********
 *
 *  Apply function to help map through the 2D array and set each 32-bit code
 *  word.
 *
 * Parameters:
 *      int col   : The current column in the array of code words.
 *      int row   : The current row in the array of code words.
 *      UArray2b_T DCT_image: The 2D blocked array of local variables.
 *      void *elem: Pointer to a struct containing the current pixel values in
 *                  the 2D blocked array.
 *      void *cl  : Pointer to the 2D array of code words.
 * 
 * Return: 
 *      None.
 *
 * Expects:
 *      A valid UArray2_T array.
 *      Properly initialized element and closure pointers.
 * 
 * Notes:
 *      Will C.R.E. if the pointer arguments are passed in as NULL.
 * 
 ************************/
void pack_image_apply(int col, int row, UArray2b_T DCT_image, void *elem, 
                                                              void *cl)
{
        (void)DCT_image;
        struct DCT_Pixel *curr_pixel = elem;
        assert(curr_pixel != NULL);
        UArray2_T packed_image = cl;
        assert(packed_image != NULL);

        /* Pack each value into the word casted as a 32-bit value */
        uint32_t *word = UArray2_at(packed_image, col, row);
        assert(word != NULL);
        
        *word = Bitpack_newu(*word, 9, 23, curr_pixel->a);
        *word = Bitpack_news(*word, 5, 18, curr_pixel->b);
        *word = Bitpack_news(*word, 5, 13, curr_pixel->c);
        *word = Bitpack_news(*word, 5, 8, curr_pixel->d);
        *word = Bitpack_newu(*word, 4, 4, curr_pixel->Pb_avg);
        *word = Bitpack_newu(*word, 4, 0, curr_pixel->Pr_avg);
}

/********** unpack_image ********
 *
 *  Converts each 32-bit codeword consisiting of a, b, c, d, and PB and PR 
 *  values into local variables.
 *
 * Parameters:
 *      UArray2_T packed_image: 2D array of 32-bit code words.
 * 
 * Return: 
 *      A 2D blocked array of local variables.
 *
 * Expects:
 *     A valid 2D array of code words.
 * 
 * Notes:
 *      Allocates a new 2D blocked array to store the local variables.
 *      Will C.R.E. if the 2D array is not valid.
 *      Will C.R.E. if the 2D blocked array is not allocated correctly.
 * 
 ************************/
UArray2b_T unpack_image(UArray2_T packed_image)
{
        assert(packed_image != NULL);
        UArray2b_T DCT_image = UArray2b_new(UArray2_width(packed_image),
                                          UArray2_height(packed_image), 
                                          sizeof(struct DCT_Pixel), 1);
        assert(DCT_image != NULL);

        /* Iterate through the array of DCT structs */
        UArray2_map_row_major(packed_image, unpack_image_apply, DCT_image);
        return DCT_image;
}

/********** unpack_image_apply ********
 *
 *  Apply function to help map through the 2D blocked array and get each 
 *  32-bit code word.
 *
 * Parameters:
 *      int col   : The current column in the array of code words.
 *      int row   : The current row in the array of code words.
 *      UArray2_T packed_image: The 2D array of codewords.
 *      void *elem: Pointer to each code word in the 2D blocked array.
 *      void *cl  : Pointer to the 2D blocked array of local variables.
 * 
 * Return: 
 *      None.
 *
 * Expects:
 *      A valid UArray2b_T array.
 *      Properly initialized element and closure pointers.
 * 
 * Notes:
 *      Will C.R.E. if the pointer arguments are passed in as NULL.
 * 
 ************************/
void unpack_image_apply(int col, int row, UArray2_T packed_image, void *elem, 
                                                                  void *cl)
{
        (void)packed_image;
        uint32_t *word = elem;
        assert(word != NULL);
        UArray2b_T DCT_image = cl;
        assert(DCT_image != NULL);

        struct DCT_Pixel *curr_pixel = UArray2b_at(DCT_image, col, row);
        assert(curr_pixel != NULL);

        curr_pixel->a = Bitpack_getu(*word, 9, 23);
        curr_pixel->b = Bitpack_gets(*word, 5, 18);
        curr_pixel->c = Bitpack_gets(*word, 5, 13);
        curr_pixel->d = Bitpack_gets(*word, 5, 8);
        curr_pixel->Pb_avg = Bitpack_getu(*word, 4, 4);
        curr_pixel->Pr_avg = Bitpack_getu(*word, 4, 0);
}