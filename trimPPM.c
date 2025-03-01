/*************************************************************
 *
 *                   trimPPM.c
 *
 *      Assignment: arith
 *      Edited by:  Aoife O'Reilly (aoreil02) and Griffin Faecher (gfaech01)
 *      Date:       2/28/2025
 *
 *      
 *
 **********************************************************/

#include "trimPPM.h"

/********** trimPPM ********
 *
 * 
 *
 * Parameters:
 *      Pnm_ppm ppm_image:
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
A2Methods_T trimPPM(Pnm_ppm ppm_image)
{
        assert(ppm_image != NULL);
        
        /* Get the dimensions of the original image */
        int width = ppm_image->methods->width(ppm_image->pixels);
        int height = ppm_image->methods->height(ppm_image->pixels);
        
        /* Allocate 2D blocked array with new dimensions and element size */
        A2Methods_T trimmed_image = ppm_image->methods->new_with_blocksize
                                (width, height, sizeof(struct Pnm_rgb *), 2);
        
        /* Check which dimensions need to be trimmed */
        if (width % 2 != 0 && height % 2 != 0) {
                width = width - 1;
                height = height - 1;
        } else if (width % 2 != 0 && height % 2 == 0) {
                width = width - 1;
        } else if (height % 2 != 0 && width % 2 == 0) {
                height = height - 1;
        } else {
                trimmed_image = ppm_image->pixels;
                return trimmed_image;
        }
        
        /* Verify dimensions are still valid  */
        assert(width > 0 && height > 0);

        

        /* Loop through every pixel in the ppm_image and initialize the 
           UArray2b with component video color space pixels */
        for (int row = 0; row < width; row++) {
                for (int col = 0; col < height; col++) {
                        struct Pnm_rgb *rgb_pixel = ppm_image->methods->at
                                                        (ppm_image, col, row);
                        rgb_pixel = trimmed_image->at(trimmed_image, col, row);
                        (void)rgb_pixel;
                }
        }

        return trimmed_image;

}