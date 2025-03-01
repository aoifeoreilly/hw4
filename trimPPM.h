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

/********** UArray2_T ********
 *
 * This structure represents a component video color space pixel.
 *
 * Elements:
 *      float Y  : represents the brightness of a color
 *      float Pb : color-difference signal proportional to Blue − Yellow
 *      float Pr : color-difference signal proportional to Red − Yellow
 * 
 ************************/
struct CompVidPixel {
        float Y;
        float Pb;
        float Pr;
};

#include <stdio.h>
#include "compress40.h"
#include "pnm.h"
#include "assert.h"
#include "a2methods.h"
#include "a2blocked.h"
#include "uarray2b.h"

UArray2b_T trimPPM(Pnm_ppm ppm_image);


#endif