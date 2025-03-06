/*************************************************************
 *
 *                      DCT_transform.c
 *
 *      Assignment: arith
 *      Edited by:  Aoife O'Reilly (aoreil02) and Griffin Faecher (gfaech01)
 *      Date:       3/4/2025
 *
 *      Converts the pixels of the given image between component video
 *      color space and their discrete cosine transform equivalents.
 *      Transforms the float values into signed and unsigned integers and
 *      vice versa.
 *
 **********************************************************/

#include "DCT_transform.h"

#define MAX_BCD 0.3

/********** CVS_to_DCT ********
 *
 *  Converts the pixels in the given 2D blocked array from component video
 *  color space to the cosine coeffecients a, b, c, and d using the discrete 
 *  cosine transform. Pb and Pr remain the same.
 *
 * Parameters:
 *      UArray2b_T averageCVS:
 * 
 * Return: 
 *      A 2D blocked array 
 *
 * Expects:
 *     A valid 2D blocked array.
 * 
 * Notes:
 *      Will C.R.E if the given UArray2b_T is not valid.
 *      Allocates a new 2D blocked array.
 *      Will C.R.E if the new 2D blocked array is not allocated properly.
 * 
 ************************/
UArray2b_T CVS_to_DCT(UArray2b_T averageCVS)
{
        assert(averageCVS != NULL);
        UArray2b_T DCT_array = UArray2b_new(UArray2b_width(averageCVS),
                                            UArray2b_height(averageCVS), 
                                            sizeof(struct DCT_Pixel),
                                            1);

        assert(DCT_array != NULL);

        /* Iterate through given array of CVS pixels */
        UArray2b_map(averageCVS, CVS_to_DCT_Apply, DCT_array);
        return DCT_array;
}

/********** CVS_to_DCT_Apply ********
 *
 *  Apply function to help map through the 2D blocked array of component video
 *  space pixels and converts 'a' to a 9-bit unsigned integer, and 'b', 'c', 
 *  and 'd' to 5-bit signed integers using the discrete cosine transform.
 *
 * Parameters:
 *      UArray2b_T averageCVS: A 2D blocked array of 
 * 
 * Return: 
 *      None.
 *
 * Expects:
 *      A valid UArray2b_T array.
 *      Properly initialized element and closure pointers.
 * 
 * Notes:
 *      Clamps the CVS floats between -0.3 and +0.3.
 *      Will C.R.E. if the pointer arguments are passed in as NULL.
 * 
 ************************/
void CVS_to_DCT_Apply(int col, int row, UArray2b_T averageCVS, void *elm, 
                                                               void *cl)
{
        (void)averageCVS;

        /* Get the current pixel in the averaged CVS array */
        struct AveragePixel *curr_pixel = elm;
        assert(curr_pixel != NULL);

        /* Get the new 2D blocked DCT array */
        UArray2b_T DCT_array = cl;
        assert(DCT_array != NULL);

        float a_float, b_float, c_float, d_float;
        struct DCT_Pixel *new_pixel = UArray2b_at(DCT_array, col, row);
        (void)new_pixel;
        
        a_float = (curr_pixel->Y4 + curr_pixel->Y3 + curr_pixel->Y2 + 
                                                     curr_pixel->Y1) / 4.0;
        b_float = (curr_pixel->Y4 + curr_pixel->Y3 - curr_pixel->Y2 - 
                                                     curr_pixel->Y1) / 4.0;
        c_float = (curr_pixel->Y4 - curr_pixel->Y3 + curr_pixel->Y2 - 
                                                     curr_pixel->Y1) / 4.0;
        d_float = (curr_pixel->Y4 - curr_pixel->Y3 - curr_pixel->Y2 + 
                                                     curr_pixel->Y1) / 4.0;

        /* Clamp the floats between -0.3 and +0.3 */
        if (b_float < -MAX_BCD) {
                b_float = -MAX_BCD;
        }
        if (b_float > MAX_BCD) {
                b_float = MAX_BCD;
        }
        if (c_float < -MAX_BCD) {
                c_float = -MAX_BCD;
        }
        if (c_float > MAX_BCD) {
                c_float = MAX_BCD;
        }
        if (d_float < -MAX_BCD) {
                d_float = -MAX_BCD;
        }
        if (d_float > MAX_BCD) {
                d_float = MAX_BCD;
        }

        /* Convert 'a' to a 9-bit signed integer */
        signed a_signed = (signed)round(a_float * 511);

        /* Convert 'b', 'c', and 'd' to 5-bit signed integers */
        signed b_signed = (signed)round((b_float * 50));
        signed c_signed = (signed)round((c_float * 50));
        signed d_signed = (signed)round((d_float * 50));

        /* Convert 'a' to 9-bit unsigned int and store all values */
        new_pixel->a = (unsigned)a_signed;
        new_pixel->b = b_signed;
        new_pixel->c = c_signed;
        new_pixel->d = d_signed;
        new_pixel->Pb_avg = curr_pixel->Pb_avg;
        new_pixel->Pr_avg = curr_pixel->Pr_avg;
}

/********** DCT_to_CVS ********
 *
 *  Converts the pixels in the given 2D blocked array from cosine coefficient
 *  elements to component video space using the inverse discrete cosine 
 *  transform. Pb and Pr remain the same.
 *
 * Parameters:
 *      UArray2b_T DCT_array: A 2D blocked array of cosine coefficients.
 * 
 * Return: 
 *      A 2D blocked array with component video space pixels.
 *
 * Expects:
 *     A valid 2D blocked array.
 * 
 * Notes:
 *      Will C.R.E if the given UArray2b_T is not valid.
 *      Allocates a new 2D blocked array.
 *      Will C.R.E if the new 2D blocked array is not allocated properly.
 * 
 ************************/
UArray2b_T DCT_to_CVS(UArray2_T DCT_array)
{
        assert(DCT_array != NULL);
        UArray2b_T averageCVS = UArray2b_new(UArray2_width(DCT_array),
                                            UArray2_height(DCT_array), 
                                            sizeof(struct AveragePixel),
                                            1);
        assert(averageCVS != NULL);

        /* Iterate through given array of CVS pixels */
        UArray2_map_row_major(DCT_array, DCT_to_CVS_Apply, averageCVS);
        return averageCVS;
}

/********** DCT_to_CVS_Apply ********
 *
 *  Apply function to help map through the 2D blocked array of component video
 *  space pixels and computes Y1, Y2, Y3, and Y4 from a, b, c, and d using
 *  the inverse discrete cosine transform.
 *
 * Parameters:
 *      UArray2b_T DCT_array: A 2D blocked array of cosine coefficient
 *                            elements.
 * 
 * Return: 
 *      None.
 *
 * Expects:
 *      A valid UArray2b_T array.
 *      Properly initialized element and closure pointers.
 * 
 * Notes:
 *      Clamps the CVS floats between -0.3 and +0.3.
 *      Will C.R.E. if the pointer arguments are passed in as NULL.
 * 
 ************************/
void DCT_to_CVS_Apply(int col, int row, UArray2_T DCT_array, void *elm, 
                                                              void *cl)
{
        (void)DCT_array;
        struct DCT_Pixel *curr_pixel = elm;
        assert(curr_pixel != NULL);
        UArray2b_T averageCVS = cl;
        assert(averageCVS != NULL);

        struct AveragePixel *new_pixel = UArray2b_at(averageCVS, col, row);
        assert(new_pixel != NULL);

        /* Convert a, b, c, and d back to floats */
        float a_float = (float)curr_pixel->a / 511.0;
        float b_float = (float)curr_pixel->b / 50.0;
        float c_float = (float)curr_pixel->c / 50.0;
        float d_float = (float)curr_pixel->d / 50.0;

        /* Use inverse DCT to get Y1, Y2, Y3, and Y4 */
        new_pixel->Y1 = a_float - b_float - c_float + d_float;
        new_pixel->Y2 = a_float - b_float + c_float - d_float;
        new_pixel->Y3 = a_float + b_float - c_float - d_float;
        new_pixel->Y4 = a_float + b_float + c_float + d_float;

        /* Get Pb and Pr */
        new_pixel->Pb_avg = curr_pixel->Pb_avg;
        new_pixel->Pr_avg = curr_pixel->Pr_avg;
}