/*************************************************************
 *
 *                   read_write.h
 *
 *      Assignment: arith
 *      Edited by:  Aoife O'Reilly (aoreil02) and Griffin Faecher (gfaech01)
 *      Date:       2/28/2025
 *
 *      Contains the function definitions to read and write the compressed 
 *      binary image to and from standard output.
 *
 **********************************************************/

#ifndef READ_WRITE_H
#define READ_WRITE_H

#include <stdio.h>
#include "compress40.h"
#include "pnm.h"
#include "assert.h"
#include "a2methods.h"
#include "uarray2.h"
#include "bitpack.h"
#include "compress_decompress.h"

void write_out(UArray2_T packed_image, int trimmed_width, int trimmed_height);
void write_out_apply(int col, int row, UArray2_T packed_image, void *elem, 
                                                                void *cl);
UArray2_T read_input(FILE *input);
void read_input_apply(int col, int row, UArray2_T packed_image, void *elem, 
                                                                void *cl);

#endif