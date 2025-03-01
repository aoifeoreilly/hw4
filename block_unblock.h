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

void calculateRGB(float Y, float Pb_avg, float Pr_avg, unsigned denominator);

#endif