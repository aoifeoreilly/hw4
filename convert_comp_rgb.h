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
 ************************/

#ifndef CONVERT_COMP_RGB_H
#define CONVERT_COMP_RGB_H

#include <stdio.h>
#include "compress40.h"
#include "pnm.h"
#include "assert.h"
#include "a2methods.h"
#include "a2blocked.h"
#include "uarray2b.h"
#include "uarray2.h"
#include "compress_decompress.h"
#include "trimPPM.h"

void rgbToCompVid(UArray2b_T comp_vid_image, Pnm_ppm ppm_image);
void rgbToCompVidApply(int col, int row, A2Methods_Object *array2b, void *elem, 
                       void *cl);
void CompVidtoRGB(UArray2b_T comp_vid_image, Pnm_ppm ppm_image);
void CompVidtoRGBApply(int col, int row, A2Methods_Object *array2b, void *elem, 
                       void *cl);

#endif