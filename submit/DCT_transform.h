/**************************************************************
 *
 *                     DCT_transform.h
 *
 *     Assignment: arith
 *     Authors:    Griffin Faecher (gfaech01) and Aoife O'Reilly (aoreil02)
 *     Date:       3/4/2025
 *
 *     Contains the function declarations to convert the pixels in a given 
 *     array between component video space and their cosine coefficient 
 *     equivalents using the discrete cosine transform and its inverse.
 *
 *************************************************************/

#ifndef DCT_TRANSFORM_H
#define DCT_TRANSFORM_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "assert.h"
#include "a2methods.h"
#include "compress_decompress.h"

/********** DCT_Pixel ********
 *
 * This structure represents a pixel with cosine coefficient elements,
 * after being converted from floats using the discrete cosine transform.
 *
 * Elements:
 *      unsigned a              : nine-bit unsigned cosine coefficient.
 *      signed b, c, d          : five-bit signed cosine coefficients.
 *      unsigned Pb_avg, Pr_avg : four-bit unsigned indeces.
 * 
 ************************/
struct DCT_Pixel {
        unsigned a;
        signed b, c, d;
        unsigned Pb_avg, Pr_avg;
};

/******************************************
 *             COMPRESSION 
*******************************************/
UArray2b_T CVS_to_DCT(UArray2b_T averageCVS);
void CVS_to_DCT_Apply(int col, int row, UArray2b_T averageCVS, void *elem, 
                                                               void *cl);

/******************************************
 *             DECOMPRESSION
*******************************************/
UArray2b_T DCT_to_CVS(UArray2b_T DCT_array);
void DCT_to_CVS_Apply(int col, int row, UArray2b_T DCT_array, void *elem, 
                                                              void *cl);

#endif