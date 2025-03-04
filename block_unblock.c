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

/********** average4to1 ********
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
UArray2b_T average4to1(UArray2b_T CVS_image)
{
        /* Allocate new 2D blocked array to hold averaged CVS values */
        UArray2b_T averageBlock = UArray2b_new(
                                        UArray2b_width(CVS_image) / BLOCKSIZE,
                                        UArray2b_height(CVS_image) / BLOCKSIZE,
                                        sizeof(struct AveragePixel),
                                        BLOCKSIZE);
        
        /* Iterate through the array of AveragePixel structs */
        UArray2b_map(averageBlock, average4to1Apply, CVS_image);

        
        return averageBlock;

}

/********** average4to1Apply ********
 *
 * 
 *
 * Parameters:
 *      void *elm: pointer to an AveragePixel struct representing a pixel
 *      void *cl:  pointer to a 2D blocked array of CVS pixels 
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
void average4to1Apply(int col, int row, UArray2b_T averageBlock, void *elm, void *cl)
{
        (void)averageBlock;
        struct AveragePixel *curr_pixel = elm;
        assert(curr_pixel != NULL);
        UArray2b_T CVS_image = cl;

        curr_pixel->Pb_avg = 0;
        curr_pixel->Pr_avg = 0;

        /* Iterate through the original array of CVS pixels */
        for (int row_offset = 0; row_offset < BLOCKSIZE; row_offset++) {
                for (int col_offset = 0; col_offset < BLOCKSIZE; col_offset++) {

                        /* Get the CVS pixel at each pixel in the 2x2 block */
                        struct CompVidPixel *old_pixel = UArray2b_at(CVS_image, 
                                                (col * BLOCKSIZE) + col_offset, 
                                                (row * BLOCKSIZE) + row_offset);
                        assert(old_pixel != NULL);
                        
                        /* Sum the Pb and Pr values in the 2x2 */
                        curr_pixel->Pb_avg += old_pixel->Pb;
                        curr_pixel->Pr_avg += old_pixel->Pr;

                        /* Set the corresponding Y values */
                        if (row_offset == 0 && col_offset == 0) {
                                curr_pixel->Y1 = old_pixel->Y;
                        } else if (row_offset == 0 && col_offset == 1) {
                                curr_pixel->Y2 = old_pixel->Y;
                        } else if (row_offset == 1 && col_offset == 0) {
                                curr_pixel->Y3 = old_pixel->Y;
                        } else {
                                curr_pixel->Y4 = old_pixel->Y;
                        }
                }
        }

        /* If the sum is not zero, divide by 4 to get the average */
        if (curr_pixel->Pb_avg != 0) {
                curr_pixel->Pb_avg /= (BLOCKSIZE * BLOCKSIZE);
        }
        if (curr_pixel->Pr_avg != 0) {
                curr_pixel->Pr_avg /= (BLOCKSIZE * BLOCKSIZE);
        }
}


/********** average1to4 ********
 *
 * 
 *
 * Parameters:
 *      UArray2b_T averageBlock:
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
UArray2b_T average1to4(UArray2b_T averageBlock)
{
        /* Allocate new 2D blocked array to hold 2x2 blocks of CVS pixels */
        UArray2b_T CVS_image = UArray2b_new(
                               UArray2b_width(averageBlock) * BLOCKSIZE,
                               UArray2b_height(averageBlock) * BLOCKSIZE,
                               sizeof(struct CompVidPixel),
                               BLOCKSIZE);
        
        /* Iterate through the array of AveragePixel structs */
        UArray2b_map(averageBlock, average1to4Apply, CVS_image);
        return CVS_image;
}

/********** average1to4 ********
 *
 * 
 *
 * Parameters:
 *      void *cl: points to a 2D blocked array of CVS pixels with the same size
                  as the original CVS array
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
void average1to4Apply(int col, int row, UArray2b_T averageBlock, void *elm, void *cl)
{
        struct AveragePixel *curr_pixel = elm;
        UArray2b_T CVS_image = cl;
        (void)averageBlock;

        /* Iterate through the 2x2 array of CVS pixels */
        for (int row_offset = 0; row_offset < BLOCKSIZE; row_offset++) {
                for (int col_offset = 0; col_offset < BLOCKSIZE; col_offset++) {
                        struct CompVidPixel *new_pixel = UArray2b_at(CVS_image, 
                                                        (col * 2) + col_offset, 
                                                        (row * 2) + row_offset);
                        new_pixel->Pb = curr_pixel->Pb_avg;
                        new_pixel->Pr = curr_pixel->Pr_avg;

                        /* Set the corresponding Y values */
                        if(row_offset == 0 && col_offset == 0) {
                                new_pixel->Y = curr_pixel->Y1;
                        } else if (row_offset == 0 && col_offset == 1) {
                                new_pixel->Y = curr_pixel->Y2;
                        } else if (row_offset == 1 && col_offset == 0) {
                                new_pixel->Y = curr_pixel->Y3;
                        } else {
                                new_pixel->Y = curr_pixel->Y4;
                        }
                }
        }
}
