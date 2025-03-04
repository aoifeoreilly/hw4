/**************************************************************
 *
 *                     block_unblock.h
 *
 *     Assignment: arith
 *     Authors:    Griffin Faecher (gfaech01) and Aoife O'Reilly (aoreil02)
 *     Date:       3/1/2025
 *
 *     
 *
 *************************************************************/

#ifndef BLOCK_UNBLOCK_H
#define BLOCK_UNBLOCK_H

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
#include "convert_comp_rgb.h"
#define BLOCKSIZE 2

/********** AveragePixel ********
 *
 * 
 *
 * Elements:
 *      float Y1, Y2, Y3, Y4 : 
 *      float Pr_avg, Pb_avg :
 * 
 ************************/
struct AveragePixel {
        float Y1, Y2, Y3, Y4;
        float Pr_avg, Pb_avg;
};

/******************************************
 *             COMPRESSION 
*******************************************/
UArray2b_T average4to1(UArray2b_T CVS_image);
void average4to1Apply(int col, int row, UArray2b_T averageBlock, void *elm, void *cl);

/******************************************
 *             DECOMPRESSION
*******************************************/
UArray2b_T average1to4(UArray2b_T averageBlock);
void average1to4Apply(int col, int row, UArray2b_T averageBlock, void *elm, void *cl);

#endif