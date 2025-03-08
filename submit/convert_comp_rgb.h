/**************************************************************
 *
 *                     convert_comp_rgb.h
 *
 *     Assignment: arith
 *     Authors:    Griffin Faecher (gfaech01) and Aoife O'Reilly (aoreil02)
 *     Date:       2/28/2025
 *
 *     Contains function declarations for converting an image between RGB 
 *     and component video space.
 *
 *************************************************************/

#ifndef CONVERT_COMP_RGB_H
#define CONVERT_COMP_RGB_H

#include <stdio.h>
#include <stdlib.h>
#include "pnm.h"
#include "assert.h"
#include "a2methods.h"
#include "compress_decompress.h"
#include "uarray2b.h"

/********** CompVidPixel ********
 *
 * This structure represents a component video color space pixel.
 *
 * Elements:
 *      float Y  : represents the brightness of a color.
 *      float Pb : color-difference signal proportional to Blue - Yellow.
 *      float Pr : color-difference signal proportional to Red - Yellow.
 * 
 ************************/
struct CompVidPixel {
        float Y;
        float Pb;
        float Pr;
};

/********** RGB_CVS_Closure ********
 *
 * This structure allows us to pass in an image and denominator to the 
 * specified apply function.
 *
 * Elements:
 *      A2Methods_T image    : For compression   - image represents the trimmed
 *                                                 array containing RGB pixels.
 *                             For decompression - image represents the CVS 
 *                                              array containing CompVidPixels.
 *      unsigned denominator : the maximum value of the image.
 * 
 ************************/
struct RGB_CVS_Closure {
        UArray2b_T image;
        unsigned denominator;
};

/******************************************
 *             COMPRESSION 
*******************************************/
UArray2b_T rgbToCompVid(UArray2b_T trimmed_image, unsigned denominator);
void rgbToCompVidApply(int col, int row, UArray2b_T array2b, void *elem, 
                       void *cl);

/******************************************
 *             DECOMPRESSION
*******************************************/
Pnm_ppm CompVidtoRGB(UArray2b_T CVS_image, A2Methods_T methods, unsigned 
                                                                denominator);
void CompVidtoRGBApply(int col, int row, UArray2b_T array2b, void *elem, 
                       void *cl);

#endif