/*************************************************************
 *
 *                 convert_comp_rgb.c
 *
 *      Assignment: arith
 *      Edited by:  Aoife O'Reilly (aoreil02) and Griffin Faecher (gfaech01)
 *      Date:       2/28/2025
 *
 *      TODO
 *
 **********************************************************/

#include "convert_comp_rgb.h"

/********** rgbToCompVid ********
 *
 * Transforms each pixel in the given array from RGB color space to component 
 * video color space.
 *
 * Parameters:
 *      A2Methods_T trimmed_image : trimmed array containing RGB pixels
 *      unsigned denominator      : the maximum value of the given ppm_image
 * 
 * Return:
 *      A2Methods_T: The array with all of its pixels in component video 
 *                   color space.
 *
 * Expects:
 *      TODO
 *
 * Notes:
 *      Converts unsigned values to a floating-point representation.
 *     
 ************************/
A2Methods_T rgbToCompVid(A2Methods_T trimmed_image, unsigned denominator) 
{
        /* cl struct to pass trimmed image & denominator into apply function */
        struct RGBtoCVS_Closure cl = {
                trimmed_image, 
                denominator
        };

        /* Create blocked 2D array of memory cells, each of size CompVidPixel */
        A2Methods_T CVS_image = trimmed_image->new_with_blocksize(
                                        trimmed_image->width(trimmed_image), 
                                        trimmed_image->height(trimmed_image), 
                                        sizeof(struct CompVidPixel *),
                                        2);

        /* Visit every cell in CVS_image and call the apply function */
        CVS_image->map_default(CVS_image, rgbToCompVidApply, &cl);
        return CVS_image;
                                        
}

void rgbToCompVidApply(int col, int row, A2Methods_Object *array2d, void *elem, 
                       void *cl)
{
        (void)array2d;

        /* Get the current CVS pixel (struct containing Y, Pb, Pr) */
        struct CompVidPixel *curr_CVS_pixel = elem;

        /* Extract trimmed image and denominator from closure */
        struct RGBtoCVS_Closure *closure = cl;
        A2Methods_T trimmed_image = closure->trimmed_image;
        unsigned denominator = closure->denominator;

        /* Get each RGB pixel from the trimmed image */
        Pnm_rgb curr_RGB_pixel = trimmed_image->at(trimmed_image, col, row);

        /* Convert to RGB values to floats */
        float red_float = curr_RGB_pixel->red / denominator;
        float green_float = curr_RGB_pixel->green / denominator;
        float blue_float = curr_RGB_pixel->blue / denominator;

        /* Calculate values for Y, Pb, and Pr */
        curr_CVS_pixel->Y = (0.299 * red_float) + 
                            (0.587 * green_float) + 
                            (0.114 * blue_float);
        curr_CVS_pixel->Pb = (-0.168736 * red_float) - 
                             (0.331264 * green_float) + 
                             (0.5 * blue_float);
        curr_CVS_pixel->Pr = (0.5 * red_float) - 
                             (0.418688 * green_float) - 
                             (0.081312 * blue_float);

        /* Clamp Y, Pb, and Pr so that they remain in bounds (lossy step) */
        if (curr_CVS_pixel->Pb < -0.5) {
                curr_CVS_pixel->Pb = -0.5;
        } else if (curr_CVS_pixel->Pb > 0.5){
                curr_CVS_pixel->Pb = 0.5;
        }
        
        if (curr_CVS_pixel->Pr < -0.5) {
                curr_CVS_pixel->Pr = -0.5;
        } else if (curr_CVS_pixel->Pr > 0.5){
                curr_CVS_pixel->Pr = 0.5;
        }

        if (curr_CVS_pixel->Y < 0) {
                curr_CVS_pixel->Y = 0;
        } else if (curr_CVS_pixel->Y > 1) {
                curr_CVS_pixel->Y = 1;
        }
}


Pnm_ppm CompVidtoRGB(A2Methods_T CVS_image, unsigned denominator)
{
        
        struct Pnm_ppm ppm_image = {
                CVS_image->width(CVS_image),
                CVS_image->height(CVS_image),
                denominator,
                CVS_image->new_with_blocksize(CVS_image->width(CVS_image), 
                                              CVS_image->height(CVS_image), 
                                              sizeof(struct Pnm_rgb *), 
                                              2),
                CVS_image
        };

        /* Visit every cell in the ppm_image pixmap and call apply function */
        ppm_image.methods->map_default(ppm_image.pixels, CompVidtoRGBApply, 
                                        &ppm_image);

        
        return ppm_image;
} 

void CompVidtoRGBApply(int col, int row, A2Methods_Object *array2b, void *elem, 
                       void *cl)
{
        (void)array2b;
        struct AverageCompVidPixel *avg_comp_vid_pixel= elem;
        struct Pnm_ppm *ppm_image = cl;

        float Y1_float = avg_comp_vid_pixel->Y1;
        float Y2_float = avg_comp_vid_pixel->Y2;
        float Y3_float = avg_comp_vid_pixel->Y3;
        float Y4_float = avg_comp_vid_pixel->Y4;
        float Pb_avg = avg_comp_vid_pixel->Pb;
        float Pr_avg = avg_comp_vid_pixel->Pr;
        
        unsigned denominator = ppm_image->denominator;

        calculateRGB(avg_comp_vid_pixel->Y1, Pb_avg, Pr_avg, denominator);
        calculateRGB(avg_comp_vid_pixel->Y2, Pb_avg, Pr_avg, denominator);
        calculateRGB(avg_comp_vid_pixel->Y3, Pb_avg, Pr_avg, denominator);
        calculateRGB(avg_comp_vid_pixel->Y4, Pb_avg, Pr_avg, denominator);
}


void calculateRGB(float Y, float Pb_avg, float Pr_avg, unsigned denominator)
{
        unsigned r = (unsigned)(((1.0 * Y) + (0.0 * Pb_avg) + 
                                 (1.402 * Pr_avg)) * denominator);

        unsigned g = (unsigned)(((1.0 * Y) + (0.344136 * Pb_avg) + 
                                 (0.714136 * Pr_avg)) * denominator);

        unsigned b = (unsigned)(((1.0 * Y) + (1.772 * Pb_avg) + 
                                 (0.0 * Pr_avg)) * denominator);                          
        

}
