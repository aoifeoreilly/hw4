/**************************************************************
 *
 *                     block_unblock.h
 *
 *     Assignment: arith
 *     Authors:    Griffin Faecher (gfaech01) and Aoife O'Reilly (aoreil02)
 *     Date:       3/1/2025
 *
 *     Contains the function declarations for the methods that will help 
 *     average the chroma elements of a 2x2 block and perform quantization.
 *
 *************************************************************/

#ifndef BLOCK_UNBLOCK_H
#define BLOCK_UNBLOCK_H

#include <stdio.h>
#include <stdlib.h>
#include "assert.h"
#include "a2methods.h"
#include "convert_comp_rgb.h"
#include "arith40.h"

/********** AveragePixel ********
 *
 * This structs holds the component video space values with averaged chroma
 * elements that are stored in each element of the averageCVS array.
 * 
 * Elements:
 *      float Y1, Y2, Y3, Y4    : the four brightness values.
 *      unsigned Pr_avg, Pb_avg : the averaged chroma elements.
 * 
 ************************/
struct AveragePixel {
        float Y1, Y2, Y3, Y4;
        unsigned Pr_avg, Pb_avg;
};

/******************************************
 *             COMPRESSION 
*******************************************/
UArray2b_T average4to1(UArray2b_T CVS_image);
void average4to1Apply(int col, int row, UArray2b_T averageCVS, void *elm,
                                                               void *cl);

/******************************************
 *             DECOMPRESSION
*******************************************/
UArray2b_T average1to4(UArray2b_T averageCVS);
void average1to4Apply(int col, int row, UArray2b_T averageCVS, void *elm, 
                                                               void *cl);

#endif