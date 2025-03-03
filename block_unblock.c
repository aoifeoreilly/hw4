/*************************************************************
 *
 *                 block_unblock.c
 *
 *      Assignment: arith
 *      Edited by:  Aoife O'Reilly (aoreil02) and Griffin Faecher (gfaech01)
 *      Date:       2/28/2025
 *
 *      
 *
 **********************************************************/

#include "block_unblock.h"

/********** compression ********
 *
 * Takes the average value of the four pixels in the block just for the Pb 
 * and Pr (chroma) elements of the pixels.
 *
 * Parameters:
 *      UArray2b_T CVS_image : 
 *      A2Methods_T methods  : 
 * 
 * Return:
 *      None
 *
 * Expects:
 *      
 *
 * Notes:
 *     
 ************************/
UArray2b_T average4to1(UArray2b_T CVS_image, A2Methods_T methods)
{
        UArray2b_T averageBlock = UArray2b_new(UArray2b_width(CVS_image) / 2,
                                               UArray2b_height(CVS_image) / 2,
                                               sizeof(struct AveragePixel),
                                               1);
        
        UArray2b_map(averageBlock, average4to1Apply, &CVS_image);
        (void)methods;

}

void average4to1Apply(int col, int row, UArray2b_T averageBlock, void *elm, void *cl)
{
        struct AveragePixel *curr_pixel = elm;
        UArray2b_T CVS_image = cl;
        
        int width = UArray2b_width(CVS_image);
        int height = UArray2b_height(CVS_image);
        (void)width;
        (void)height;
        (void)curr_pixel;
        (void)averageBlock;

        // struct CompVidPixel *pix1 = UArray2b_at(CVS_image, col, row);
        // struct CompVidPixel *pix2 = UArray2b_at(CVS_image, col + 1, row);
        // struct CompVidPixel *pix3 = UArray2b_at(CVS_image, col, row + 1);
        // struct CompVidPixel *pix4 = UArray2b_at(CVS_image, col + 1, row + 1);

        // float Pb1  = pix1->Pb1;
        // float Pb2  = pix2->Pb2;
        // float Pb3  = pix3->Pb3;
        // float Pb4  = pix4->Pb4;
        // float Pb = (Pb1 + Pb2 + Pb3 + Pb4) / 4;

        float Pb_avg = 0;
        float Pr_avg = 0;
        float Y1;
        float Y2;
        float Y3;
        float Y4;

        for (int curr_row = 0; curr_row < BLOCKSIZE; curr_row++) {
                for (int curr_col = 0; curr_col < BLOCKSIZE; curr_col++) {
                        UArray2b_at(CVS_image, (col * 2) + curr_col, (row * 2) + cur_row);
                        // UArray2b_at(CVS_image, col * 2, row * 2);
                }
        }
}