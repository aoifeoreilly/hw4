/**************************************************************
 *
 *                     convert_comp_rgb.h
 *
 *     Assignment: arith
 *     Authors:    Griffin Faecher (gfaech01) and Aoife O'Reilly (aoreil02)
 *     Date:       2/28/2025
 *
 *     
 *
 *************************************************************/

#ifndef CONVERT_COMP_RGB_H
#define CONVERT_COMP_RGB_H

#include <stdio.h>
#include <stdlib.h>
#include "pnm.h"
#include "assert.h"
#include "a2methods.h"
#include "a2blocked.h"
#include "uarray2b.h"
#include "uarray2.h"
#include "compress_decompress.h"
#include "trimPPM.h"

/********** CompVidPixel ********
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

/********** RGBtoCVS_Closure ********
 *
 * This structure represents a component video color space pixel.
 *
 * Elements:
 *      A2Methods_T trimmed_image : trimmed array containing RGB pixels
 *      unsigned denominator      : the maximum value of the given ppm_image
 * 
 ************************/
struct RGBtoCVS_Closure {
        A2Methods_T trimmed_image;
        unsigned denominator;
};


/******************************************
 *             COMPRESSION 
*******************************************/
A2Methods_T rgbToCompVid(A2Methods_T trimmed_image, unsigned denominator);
void rgbToCompVidApply(int col, int row, A2Methods_Object *array2d, void *elem, 
                       void *cl);

/******************************************
 *             DECOMPRESSION
*******************************************/
Pnm_ppm CompVidtoRGB(A2Methods_T comp_vid_image, unsigned denominator);
void CompVidtoRGBApply(int col, int row, A2Methods_Object *array2b, void *elem, 
                       void *cl);
void calculateRGB(float Y, float Pb_avg, float Pr_avg, unsigned denominator);

#endif