/*************************************************************
 *
 *                 block_unblock.c
 *
 *      Assignment: arith
 *      Edited by:  Aoife O'Reilly (aoreil02) and Griffin Faecher (gfaech01)
 *      Date:       2/28/2025
 *
 *      
 *
 **********************************************************/

#include "block_unblock.h"



void calculateRGB(float Y, float Pb_avg, float Pr_avg, unsigned denominator)
{
        unsigned r = (unsigned)(((1.0 * Y) + (0.0 * Pb_avg) + 
                                 (1.402 * Pr_avg)) * denominator);

        unsigned g = (unsigned)(((1.0 * Y) + (0.344136 * Pb_avg) + 
                                 (0.714136 * Pr_avg)) * denominator);

        unsigned b = (unsigned)(((1.0 * Y) + (1.772 * Pb_avg) + 
                                 (0.0 * Pr_avg)) * denominator);                          
        

}