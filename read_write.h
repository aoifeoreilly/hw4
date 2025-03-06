/*************************************************************
 *
 *                   read_write.h
 *
 *      Assignment: arith
 *      Edited by:  Aoife O'Reilly (aoreil02) and Griffin Faecher (gfaech01)
 *      Date:       2/28/2025
 *
 *      
 *
 **********************************************************/

#ifndef READ_WRITE_H
#define READ_WRITE_H

#include <stdio.h>
#include "compress40.h"
#include "pnm.h"
#include "assert.h"
#include "a2methods.h"
#include "compress_decompress.h"

void write(UArray2b_T packed_image, int trimmed_width, int trimmed_height);
UArray2b_T read(FILE *input);

#endif