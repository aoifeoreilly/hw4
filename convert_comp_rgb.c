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
UArray2b_T rgbToCompVid(UArray2b_T trimmed_image, unsigned denominator) 
{
        assert(trimmed_image != NULL);

        /* cl struct to pass trimmed image & denominator into apply function */
        struct RGB_CVS_Closure cl = {
                trimmed_image,
                denominator
        };
        

        UArray2b_T CVS_image = UArray2b_new(UArray2b_width(trimmed_image),
                                        UArray2b_height(trimmed_image), 
                                        sizeof(struct CompVidPixel),
                                        2);
        
        /* Visit every cell in CVS_image and call the apply function */
        UArray2b_map(CVS_image, rgbToCompVidApply, &cl);
        return CVS_image;
                                        
}

/********** rgbToCompVidApply ********
 *
 * 
 *
 * Parameters:
 *      int col                   : The current column index in the given image.
 *      int row                   : The current row index in the given image.
 *      A2Methods_Object *array2d :
 *      void *elem                :
 *      void *cl                  : A pointer to a struct containing the
 *                                  trimmed image array and image denominator.
 * 
 * Return:
 *      None
 *
 * Expects:
 *      A valid A2Methods_T image and a properly initialized closure struct.
 *
 * Notes:
 *     
 ************************/
void rgbToCompVidApply(int col, int row, UArray2b_T array2b, void *elem, 
                       void *cl)
{
        (void)array2b;

        /* Get the current CVS pixel (struct containing Y, Pb, Pr) */
        struct CompVidPixel *curr_CVS_pixel = elem;

        /* Extract trimmed image and denominator from closure */
        struct RGB_CVS_Closure *closure = cl;
        UArray2b_T trimmed_image = closure->image;
        unsigned denominator = closure->denominator;
        
        /* Get each RGB pixel from the trimmed image */
        Pnm_rgb curr_RGB_pixel = UArray2b_at(trimmed_image, col, row);

        /* Convert to RGB values to floats */
        float red_float = (float)curr_RGB_pixel->red / (float)denominator;
        float green_float = (float)curr_RGB_pixel->green / (float)denominator;
        float blue_float = (float)curr_RGB_pixel->blue / (float)denominator;

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

/********** CompVidtoRGB ********
 *
 * 
 *
 * Parameters:
 *      A2Methods_T CVS_image :
 *      unsigned denominator  : 
 * Return:
 *      Pnm_ppm:
 *
 * Expects:
 *
 * Notes:
 *     
 ************************/
Pnm_ppm CompVidtoRGB(UArray2b_T CVS_image, A2Methods_T methods, unsigned denominator)
{
        /* Initialize a ppm struct with information from the given CVS_image */
        Pnm_ppm ppm_image = malloc(sizeof(*ppm_image));

        ppm_image->width = UArray2b_width(CVS_image);
        ppm_image->height = UArray2b_height(CVS_image);
        ppm_image->denominator = denominator,
        ppm_image->pixels = UArray2b_new(UArray2b_width(CVS_image), 
                                        UArray2b_height(CVS_image), 
                                        sizeof(struct Pnm_rgb), 
                                        2);
        ppm_image->methods = methods;
        
        struct RGB_CVS_Closure cl = {
                CVS_image,
                denominator
        };
        /* Visit every cell in the ppm_image pixmap and call apply function */
        UArray2b_map(ppm_image->pixels, CompVidtoRGBApply, &cl);
        return ppm_image;
} 

/********** CompVidtoRGBApply ********
 *
 * 
 *
 * Parameters:
 *      int col                   : The current column index in the given image.
 *      int row                   : The current row index in the given image.
 *      A2Methods_Object *array2d :
 *      void *elem                :
 *      void *cl                  : A pointer to a struct containing the
 *                                  trimmed image array and image denominator.
 * 
 * Return:
 *      None.
 *
 * Expects:
 *      A valid A2Methods_T image and a properly initialized closure struct.
 * 
 * Notes:
 *     
 ************************/
void CompVidtoRGBApply(int col, int row, UArray2b_T array2b, void *elem, 
                       void *cl)
{
        (void)array2b;

        /* Get the current RGB pixel (struct containing R, G, B) */
        struct Pnm_rgb *curr_RGB_pixel = elem;

        /* Extract CVS image and chosen denominator from closure */
        struct RGB_CVS_Closure *closure = cl;
        A2Methods_UArray2 CVS_image = closure->image;
        unsigned denominator = closure->denominator;

        /* Get each CVS pixel from the trimmed image */
        struct CompVidPixel *curr_CVS_pixel = UArray2b_at(CVS_image, col, row);
        float Y  = curr_CVS_pixel->Y;
        float Pb = curr_CVS_pixel->Pb;
        float Pr = curr_CVS_pixel->Pr;

        signed red_signed = (signed)(((1.0 * Y) + (0.0 * Pb) + 
                                      (1.402 * Pr)) * denominator);

        signed green_signed = (signed)(((1.0 * Y) - (0.344136 * Pb) - 
                                        (0.714136 * Pr)) * denominator);

        signed blue_signed = (signed)(((1.0 * Y) + (1.772 * Pb) + 
                                       (0.0 * Pr)) * denominator);

        /* Clamp signed values between 0 and 255 */
        if (red_signed < 0) {
                red_signed = 0;
        } else if (red_signed > 255) {
                red_signed = 255;
        }

        if (green_signed < 0) {
                green_signed = 0;
        } else if (green_signed > 255) {
                green_signed = 255;
        }

        if (blue_signed < 0) {
                blue_signed = 0;
        } else if (blue_signed > 255) {
                blue_signed = 255;
        }
        
        curr_RGB_pixel->red = (unsigned)red_signed;
        curr_RGB_pixel->green = (unsigned)green_signed;
        curr_RGB_pixel->blue = (unsigned)blue_signed;
}
