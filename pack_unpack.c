/**************************************************************
 *
 *                     pack_unpack.c
 *
 *      Assignment: arith
 *      Authors:    Griffin Faecher (gfaech01) and Aoife O'Reilly (aoreil02)
 *      Date:       3/5/2025
 *
 *      This file
 *
 *************************************************************/

#include "pack_unpack.h"

UArray2b_T pack_image(UArray2b_T DCT_image)
{
        assert(DCT_image != NULL);
        UArray2b_T packed_image = UArray2b_new(UArray2b_width(DCT_image),
                                                UArray2b_height(DCT_image), 
                                                sizeof(uint32_t), BLOCKSIZE);
        assert(packed_image != NULL);
        UArray2b_map(DCT_image, pack_image_apply, packed_image);
        
        return packed_image;
}

void pack_image_apply(int col, int row, UArray2b_T DCT_image, void *elem, void *cl)
{
        (void)DCT_image;
        struct DCT_Pixel *curr_pixel = elem;
        UArray2b_T packed_image = cl;

        /* Pack each value into the word casted as a 32-bit value */
        /* perchance attend OH */
        uint32_t *word = UArray2b_at(packed_image, col, row);
        *word = Bitpack_newu(*word, 9, 23, curr_pixel->a);
        *word = Bitpack_news(*word, 5, 18, curr_pixel->b);
        *word = Bitpack_news(*word, 5, 13, curr_pixel->c);
        *word = Bitpack_news(*word, 5, 8, curr_pixel->d);
        *word = Bitpack_newu(*word, 4, 4, curr_pixel->Pb_avg);
        *word = Bitpack_newu(*word, 4, 0, curr_pixel->Pr_avg);
}


UArray2b_T unpack_image(UArray2b_T packed_image)
{
        assert(packed_image != NULL);
        UArray2b_T DCT_image = UArray2b_new(UArray2b_width(packed_image),
                                                 UArray2b_height(packed_image), 
                                                 sizeof(struct DCT_Pixel),
                                                 2);
        assert(DCT_image != NULL);

        /* Iterate through the array of DCT structs */
        UArray2b_map(packed_image, unpack_image_apply, DCT_image);
        return DCT_image;
}

void unpack_image_apply(int col, int row, UArray2b_T packed_image, void *elem, void *cl)
{
        (void)packed_image;
        uint32_t *word = elem;
        UArray2b_T DCT_image = cl;
        struct DCT_Pixel *curr_pixel = UArray2b_at(DCT_image, col, row);
        
        curr_pixel->a = Bitpack_getu(*word, 9, 23);
        curr_pixel->b = Bitpack_gets(*word, 5, 18);
        curr_pixel->c = Bitpack_gets(*word, 5, 13);
        curr_pixel->d = Bitpack_gets(*word, 5, 8);
        curr_pixel->Pb_avg = Bitpack_getu(*word, 4, 4);
        curr_pixel->Pr_avg = Bitpack_getu(*word, 4, 0);
}