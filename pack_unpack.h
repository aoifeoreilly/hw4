/**************************************************************
 *
 *                     pack_unback.h
 *
 *     Assignment: arith
 *     Authors:    Griffin Faecher (gfaech01) and Aoife O'Reilly (aoreil02)
 *     Date:       3/5/2025
 *
 *     
 *
 *************************************************************/

#ifndef PACK_UNPACK_H
#define PACK_UNPACK_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "assert.h"
#include "a2methods.h"
#include "compress_decompress.h"

/******************************************
 *             COMPRESSION 
*******************************************/
UArray2b_T pack_image(UArray2b_T DCT_image);
void pack_image_apply(int col, int row, UArray2b_T DCT_image, void *elem, void *cl);

/******************************************
 *             DECOMPRESSION 
*******************************************/
UArray2b_T unpack_image(UArray2b_T packed_image);
void unpack_image_apply(int col, int row, UArray2b_T packed_image, void *elem, void *cl);

#endif