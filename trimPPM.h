/*************************************************************
 *
 *                   trimPPM.h
 *
 *      Assignment: arith
 *      Edited by:  Aoife O'Reilly (aoreil02) and Griffin Faecher (gfaech01)
 *      Date:       2/28/2025
 *
 *      
 *
 **********************************************************/

#ifndef TRIMPPM_H
#define TRIMPPM_H

#include <stdio.h>
#include "compress40.h"
#include "pnm.h"
#include "assert.h"
#include "a2methods.h"
#include "a2blocked.h"
#include "uarray2b.h"
#include "compress_decompress.h"

UArray2b_T trimPPM(Pnm_ppm ppm_image);

#endif