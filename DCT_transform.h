/**************************************************************
 *
 *                     DCT_transform.h
 *
 *     Assignment: arith
 *     Authors:    Griffin Faecher (gfaech01) and Aoife O'Reilly (aoreil02)
 *     Date:       3/4/2025
 *
 *     
 *
 *************************************************************/

#ifndef DCT_TRANSFORM_H
#define DCT_TRANSFORM_H

#include <stdio.h>
#include <stdlib.h>
#include "assert.h"
#include "a2methods.h"
#include "compress_decompress.h"

/********** DCT_Pixel ********
 *
 * This structure represents a 
 *
 * Elements:
 *      unsigned a              : 
 *      signed b, c, d          : 
 *      unsigned Pb_avg, Pr_avg : 
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
void CVS_to_DCT_Apply(int col, int row, UArray2b_T averageCVS, void *elm, void *cl);

/******************************************
 *             DECOMPRESSION
*******************************************/
UArray2b_T DCT_to_CVS(UArray2b_T averageCVS);
void DCT_to_CVS_Apply(int col, int row, UArray2b_T averageCVS, void *elm, void *cl);

#endif