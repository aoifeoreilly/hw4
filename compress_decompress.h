/*************************************************************
 *
 *                   compress_decompress.h
 *
 *      Assignment: arith
 *      Edited by:  Aoife O'Reilly (aoreil02) and Griffin Faecher (gfaech01)
 *      Date:       2/28/2025
 *
 *      Contains the function declarations for the compress_decompress 
 *      interface.
 *
 **********************************************************/

#ifndef COMPRESS_DECOMPRESS_H
#define COMPRESS_DECOMPRESS_H

#include <stdio.h>
#include <a2blocked.h>
#include <bitpack.h>
#include "pnm.h"
#include <a2methods.h>
#include "a2plain.h"
#include "assert.h"
#include "trimPPM.h"
#include "convert_comp_rgb.h"
#include "block_unblock.h"
#include "DCT_transform.h"
#include "pack_unpack.h"
#include "read_write.h"
#include "uarray2b.h"
#define BLOCKSIZE 2

void compress40(FILE *input);
void decompress40(FILE *input);

#endif