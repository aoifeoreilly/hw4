/*************************************************************
 *
 *                   trimPPM.c
 *
 *      Assignment: arith
 *      Edited by:  Aoife O'Reilly (aoreil02) and Griffin Faecher (gfaech01)
 *      Date:       2/28/2025
 *
 *      The purpose of this file is to trim the last row and/or column of the 
 *      provided ppm image if either of the dimensions are odd. This verifies 
 *      that the width and height of the image are now even numbers.
 *
 **********************************************************/

#include "trimPPM.h"

/********** trimPPM ********
 *
 * Checks the current dimensions of the provided ppm image and trims the last
 * row and/or column if necessary.
 *
 * Parameters:
 *      Pnm_ppm ppm_image: A full-color portable pixmap image.
 * 
 * Return:
 *      A 2D blocked array containing the possibly trimmed ppm image. 
 *
 * Expects:
 *      A valid Pnm_ppm.
 *
 * Notes:
 *      Will C.R.E if the given Pnm_ppm is not valid.
 *      Allocates a new 2D blocked array with possibly new dimensions.
 *      
 ************************/
UArray2b_T trimPPM(Pnm_ppm ppm_image)
{
        assert(ppm_image != NULL);
        
        /* Get the dimensions of the original image */
        int width = ppm_image->methods->width(ppm_image->pixels);
        int height = ppm_image->methods->height(ppm_image->pixels);
        
        /* Check which dimensions need to be trimmed */
        if (width % 2 != 0 && height % 2 != 0) {
                width = width - 1;
                height = height - 1;
        } else if (width % 2 != 0 && height % 2 == 0) {
                width = width - 1;
        } else if (height % 2 != 0 && width % 2 == 0) {
                height = height - 1;
        }
        UArray2b_T trimmed_image = UArray2b_new(width, height, 
                                        sizeof(struct Pnm_rgb),
                                        BLOCKSIZE);
        assert(trimmed_image != NULL);
        
        /* Verify dimensions are still valid  */
        assert(width > 0 && height > 0);
        
        /* Loop through every pixel in the ppm_image and initialize the 
           UArray2b with component video color space pixels */
        for (int row = 0; row < height; row++) {
                for (int col = 0; col < width; col++) {
                        struct Pnm_rgb *src_pixel = ppm_image->methods->at
                                                (ppm_image->pixels, col, row);
                        struct Pnm_rgb *dest_pixel = UArray2b_at
                                                (trimmed_image, col, row);
                        *dest_pixel = *src_pixel;
                }
        }
        return trimmed_image;
}