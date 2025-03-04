/*************************************************************
 *
 *                      DCT_transform.c
 *
 *      Assignment: arith
 *      Edited by:  Aoife O'Reilly (aoreil02) and Griffin Faecher (gfaech01)
 *      Date:       3/4/2025
 *
 *      TODO
 *
 **********************************************************/

#include "DCT_transform.h"

UArray2b_T CVS_to_DCT(UArray2b_T averageCVS)
{
        UArray2b_T DCT_array = UArray2b_new(UArray2b_width(averageCVS),
                                            UArray2b_height(averageCVS), 
                                            sizeof(struct DCT_Pixel),
                                            BLOCKSIZE);

        /* Iterate through given array of CVS pixels */
        UArray2b_map(averageCVS, CVS_to_DCT_Apply, DCT_array);
        return DCT_array;
}

void CVS_to_DCT_Apply(int col, int row, UArray2b_T averageCVS, void *elm, void *cl)
{
        
        (void)averageCVS;
        struct AveragePixel *curr_pixel = elm;
        assert(curr_pixel != NULL);
        UArray2b_T DCT_array = cl;
        assert(DCT_array != NULL);
        float a_float, b_float, c_float, d_float;
        
        struct DCT_Pixel *new_pixel = UArray2b_at(DCT_array, col, row);
        a_float = (curr_pixel->Y4 + curr_pixel->Y3 + curr_pixel->Y2 + 
                                                          curr_pixel->Y1) / 4.0;
        b_float = (curr_pixel->Y4 + curr_pixel->Y3 - curr_pixel->Y2 - 
                                                          curr_pixel->Y1) / 4.0;
        c_float = (curr_pixel->Y4 - curr_pixel->Y3 + curr_pixel->Y2 - 
                                                          curr_pixel->Y1) / 4.0;
        d_float = (curr_pixel->Y4 - curr_pixel->Y3 - curr_pixel->Y2 + 
                                                          curr_pixel->Y1) / 4.0;
        
}

UArray2b_T DCT_to_CVS(UArray2b_T averageCVS)
{
        
}

void DCT_to_CVS_Apply(int col, int row, UArray2b_T averageCVS, void *elm, void *cl)
{
        
}