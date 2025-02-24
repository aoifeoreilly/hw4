/**************************************************************
 *
 *                     ppmdiff.h
 *
 *     Assignment: arith
 *     Authors:    Griffin Faecher (gfaech01) and Aoife O'Reilly (aoreil02)
 *     Date:       2/21/2025
 *
 *     This file contains the function declarations for ppmdiff.
 *
 ************************/

#ifndef PPMDIFF_H
#define PPMDIFF_H

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "assert.h"
#include "math.h"
#include "compress40.h"
#include "pnm.h"
#include "a2methods.h"
#include "a2plain.h"


static FILE *openFile(char *fname, char *mode);
double find_rms(Pnm_ppm image1, Pnm_ppm image2);
void checkBorders(Pnm_ppm image1, Pnm_ppm image2, int *height, int *width);
double findSum(Pnm_rgb pixel1, Pnm_rgb pixel2, double denominator);

#endif