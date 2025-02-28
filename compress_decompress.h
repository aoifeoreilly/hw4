/*************************************************************
 *
 *                   compress_decompress.h
 *
 *      Assignment: arith
 *      Edited by:  Aoife O'Reilly (aoreil02) and Griffin Faecher (gfaech01)
 *      Date:       2/28/2025
 *
 *      
 *
 **********************************************************/

#include <stdio.h>
#include "pnm.h"
#include "a2methods.h"
#include "a2plain.h"
#include "a2blocked.h"
#include "assert.h"
#include "trimPPM.h"

void compress40(FILE *input);
void decompress40(FILE *input);