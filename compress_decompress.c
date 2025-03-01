/*************************************************************
 *
 *                   compress_decompress.c
 *
 *      Assignment: arith
 *      Edited by:  Aoife O'Reilly (aoreil02) and Griffin Faecher (gfaech01)
 *      Date:       2/28/2025
 *
 *      
 *
 **********************************************************/

#include "compress_decompress.h"
#define DENOMINATOR 255

/********** compression ********
 *
 * 
 *
 * Parameters:
 *      FILE *input: 
 * 
 * Return:
 *      None
 *
 * Expects:
 *      
 *
 * Notes:
 *     
 ************************/
void compress40(FILE *input)
{
        /* Verify valid input file */
        assert(input != NULL);

        /* Use UArray2 blocked methods */
        A2Methods_T methods = uarray2_methods_blocked;
        assert(methods != NULL);
         
        /* Compression Step #1: Read in the ppm image */
        Pnm_ppm ppm_image = Pnm_ppmread(input, methods);
        assert(ppm_image != NULL);

        /* Compression Step #2: Allocate UArray2b_T for image with trimmed 
                                dimensions in component video color space */
        A2Methods_T trimmed_image = trimPPM(ppm_image);
        (void)trimmed_image;

        /* Compression Step #3: Convert RGB values to comp video color format */
        A2Methods_T CVS_image = rgbToCompVid(trimmed_image, ppm_image->denominator);
        (void)CVS_image;

        
}

/********** decompression ********
 *
 * 
 *
 * Parameters:
 *      FILE *input: 
 * 
 * Return:
 *      None
 *
 * Expects:
 *      
 *
 * Notes:
 *     
 ************************/
void decompress40(FILE *input)
{      
        (void)input;
        assert(input != NULL);  
        
        /* Use UArray2 blocked methods */
        A2Methods_T methods = uarray2_methods_blocked;
        assert(methods != NULL);
        
        A2Methods_T CVS_image = methods->new_with_blocksize(0, 0, 
                                             sizeof(struct CompVidPixel *), 
                                             2);    

        CompVidtoRGB(CVS_image, DENOMINATOR);  
        
        
        // Pnm_ppm final_ppm = NULL;
        // Pnm_ppmwrite(stdout, final_ppm);
        
        
}