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

/********** UArray2_T ********
 *
 * This structure represents a component video color space pixel.
 *
 * Elements:
 *      float Y  : represents the brightness of a color
 *      float Pb : color-difference signal proportional to Blue − Yellow
 *      float Pr : color-difference signal proportional to Red − Yellow
 * 
 ************************/
struct CompVidPixel {
        float Y;
        float Pb;
        float Pr;
};

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

        /* Verify dimensions are still valid  */
        assert(width > 0 && height > 0);

        /* Allocate 2D blocked array with new dimensions and element size */
        UArray2b_T comp_vid_image = UArray2b_new(width, 
                                                 height, 
                                                 sizeof(float) * 3,
                                                 2);

        /* Loop through every pixel in the ppm_image and initialize the 
           UArray2b with component video color space pixels */
        for (int row = 0; row < width; row++) {
                for (int col = 0; col < height; col++) {
                        struct CompVidPixel *pixel = 
                                        UArray2b_at(comp_vid_image, col, row);
                        pixel->Y = 0;
                        pixel->Pb = 0;
                        pixel->Pr = 0;
                }
        }

        return comp_vid_image;
}