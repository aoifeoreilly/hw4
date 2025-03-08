/**************************************************************
 *
 *                     pack_unback.h
 *
 *     Assignment: arith
 *     Authors:    Griffin Faecher (gfaech01) and Aoife O'Reilly (aoreil02)
 *     Date:       3/5/2025
 *
 *     Contains the function declarations for the methods that will help 
 *     packs and unpack the local variables and 32-bit code words.
 *
 *************************************************************/

#ifndef PACK_UNPACK_H
#define PACK_UNPACK_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "assert.h"
#include "a2methods.h"
#include "uarray2.h"
#include "compress_decompress.h"

/******************************************
 *             COMPRESSION 
*******************************************/
UArray2_T pack_image(UArray2b_T DCT_image);
void pack_image_apply(int col, int row, UArray2b_T DCT_image, void *elem, 
                                                              void *cl);

/******************************************
 *             DECOMPRESSION 
*******************************************/
UArray2b_T unpack_image(UArray2_T packed_image);
void unpack_image_apply(int col, int row, UArray2_T packed_image2, void *elem, 
                                                                   void *cl);

#endif