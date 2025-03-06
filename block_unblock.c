/*************************************************************
 *
 *                 block_unblock.c
 *
 *      Assignment: arith
 *      Edited by:  Aoife O'Reilly (aoreil02) and Griffin Faecher (gfaech01)
 *      Date:       2/28/2025
 *
 *      Takes the average value of the PB and PR (chroma) elements of the 
 *      the four pixels in the given 2x2 block and converts them to 
 *      four-bit quantized representation. Performs the reverse process as
 *      well.
 *
 **********************************************************/

#include "block_unblock.h"

/********** average4to1 ********
 *
 * Takes the average value of the four CVS pixels in the block just for the Pb 
 * and Pr (chroma) elements of the pixels. Quantizes all of the values.
 *
 * Parameters:
 *      UArray2b_T CVS_image : A 2D blocked array that holds CVS pixels.
 * 
 * Return:
 *      A 2D blocked array that holds the CVS values with averaged Pb and Pr
 *      elements.
 *
 * Expects:
 *      A valid image.
 *      The given array's width and height to be divisible by the BLOCKSIZE.
 *
 * Notes:
 *      Allocates a new 2D blocked array.
 *      Will C.R.E. if the new array is NULL after allocation.
 *      
 ************************/
UArray2b_T average4to1(UArray2b_T CVS_image)
{
        /* Allocate new 2D blocked array to hold averaged CVS values */
        UArray2b_T averageCVS = UArray2b_new(
                                        UArray2b_width(CVS_image) / BLOCKSIZE,
                                        UArray2b_height(CVS_image) / BLOCKSIZE,
                                        sizeof(struct AveragePixel), 1);
        assert(averageCVS != NULL);
        
        /* Iterate through the array of AveragePixel structs */
        UArray2b_map(averageCVS, average4to1Apply, CVS_image);

        return averageCVS;

}

/********** average4to1Apply ********
 *
 * Apply function to help iterate through the original array of CVS pixels
 * and perform the average and quantizing operations.
 *
 * Parameters:
 *      int col  : The current column in the averaged array.
 *      int row  : The current row in the averaged array.
 *      UArray2b_T averageCVS : A 2D blocked array of quantized CVS pixels 
 *                              with average Pb and Pr elements.
 *      void *elm: A pointer to an AveragePixel struct representing a pixel.
 *      void *cl : A pointer to a 2D blocked array of CVS pixels.
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
void average4to1Apply(int col, int row, UArray2b_T averageCVS, void *elm,
                                                               void *cl)
{
        (void)averageCVS;
        struct AveragePixel *curr_pixel = elm;
        assert(curr_pixel != NULL);
        UArray2b_T CVS_image = cl;
        assert(CVS_image != NULL);

        float Pb_avg = 0;
        float Pr_avg = 0;
        curr_pixel->Pb_avg = 0;
        curr_pixel->Pr_avg = 0;

        /* Iterate through the original array of CVS pixels */
        for (int rowOffset = 0; rowOffset < BLOCKSIZE; rowOffset++) {
                for (int colOffset = 0; colOffset < BLOCKSIZE; colOffset++) {

                        /* Get the CVS pixel at each pixel in the 2x2 block */
                        struct CompVidPixel *old_pixel = UArray2b_at(CVS_image, 
                                                (col * BLOCKSIZE) + colOffset, 
                                                (row * BLOCKSIZE) + colOffset);
                        assert(old_pixel != NULL);
                        
                        /* Sum the Pb and Pr values in the 2x2 */
                        Pb_avg += old_pixel->Pb;
                        Pr_avg += old_pixel->Pr;
   
                        /* Set the corresponding Y values */
                        if (rowOffset == 0 && colOffset == 0) {
                                curr_pixel->Y1 = old_pixel->Y;
                        } else if (rowOffset == 0 && colOffset == 1) {
                                curr_pixel->Y2 = old_pixel->Y;
                        } else if (rowOffset == 1 && colOffset == 0) {
                                curr_pixel->Y3 = old_pixel->Y;
                        } else {
                                curr_pixel->Y4 = old_pixel->Y;
                        }
                }
        }

        /* If the sum is not zero, divide by 4 to get the average and
           convert to 4-bit quantized representation of the chroma value */
        if (Pb_avg != 0) {
                Pb_avg /= (BLOCKSIZE * BLOCKSIZE);
        }
        curr_pixel->Pb_avg = Arith40_index_of_chroma(Pb_avg);
        if (Pr_avg != 0) {
                Pr_avg /= (BLOCKSIZE * BLOCKSIZE);
        }
        curr_pixel->Pr_avg = Arith40_index_of_chroma(Pr_avg);
}


/********** average1to4 ********
 *
 * Expands each element in the averaged array to a 2x2 block in a new array by
 * setting the correspoonding Y values and converting the chroma elements back
 * to floats.
 *
 * Parameters:
 *      UArray2b_T averageCVS : A 2D blocked array that holds the quantized
 *                              CVS pixels with averaged Pb and Pr elements.
 * 
 * Return:
 *      A 2D blocked array that holds the CVS values with the chroma floats.
 *
 * Expects:
 *      A valid 2D blocked array.
 *
 * Notes:
 *      Allocates a new 2D blocked array.
 *      Will C.R.E. if the new array is NULL after allocation.
 *      
 ************************/
UArray2b_T average1to4(UArray2b_T averageCVS)
{
        /* Allocate new 2D blocked array to hold 2x2 blocks of CVS pixels */
        UArray2b_T CVS_image = UArray2b_new(
                               UArray2b_width(averageCVS) * BLOCKSIZE,
                               UArray2b_height(averageCVS) * BLOCKSIZE,
                               sizeof(struct CompVidPixel),
                               BLOCKSIZE);
        assert(CVS_image != NULL);

        /* Iterate through the array of AveragePixel structs */
        UArray2b_map(averageCVS, average1to4Apply, CVS_image);
        return CVS_image;
}

/********** average1to4 ********
 *
 * Apply function to help iterate through the original array of CVS pixels
 * and set the Y values in the new CVS array.
 *
 * Parameters:
 *      int col  : The current column in the averaged array.
 *      int row  : The current row in the averaged array.
 *      UArray2b_T averageCVS : A 2D blocked array of quantized CVS pixels 
 *                              with average Pb and Pr elements.
 *      void *elm: A pointer to an AveragePixel struct representing a pixel.
 *      void *cl: points to a 2D blocked array of CVS pixels with the same size
                  as the original CVS array
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
void average1to4Apply(int col, int row, UArray2b_T averageCVS, void *elm,
                                                               void *cl)
{
        struct AveragePixel *curr_pixel = elm;
        UArray2b_T CVS_image = cl;
        (void)averageCVS;

        /* Iterate through the 2x2 array of CVS pixels */
        for (int row_offset = 0; row_offset < BLOCKSIZE; row_offset++) {
                for (int col_offset = 0; col_offset < BLOCKSIZE; col_offset++) {

                        /* Get the CVS pixel at the correct location */
                        struct CompVidPixel *new_pixel = UArray2b_at(CVS_image, 
                                                        (col * 2) + col_offset, 
                                                        (row * 2) + row_offset);
                        
                        new_pixel->Pb = Arith40_chroma_of_index(
                                                        curr_pixel->Pb_avg);
                        new_pixel->Pr = Arith40_chroma_of_index(
                                                        curr_pixel->Pr_avg);

                        /* Set the corresponding Y values */
                        if (row_offset == 0 && col_offset == 0) {
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
