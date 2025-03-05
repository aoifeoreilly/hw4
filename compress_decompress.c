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
        UArray2b_T trimmed_image = trimPPM(ppm_image);

        /*** PRINT TRIMMED IMAGE ***
                Pnm_ppm final_image = malloc(sizeof(*final_image));
                final_image->width = methods->width(trimmed_image);
                final_image->height = methods->height(trimmed_image);
                final_image->denominator = ppm_image->denominator;
                final_image->pixels = trimmed_image;        
                final_image->methods = methods;
        */
        
        /* Compression Step #3: Convert RGB values to comp video color format */
        UArray2b_T CVS_image = rgbToCompVid(trimmed_image, ppm_image->denominator);
        
        /* Compression Step #4: fuck that jawn up */
        UArray2b_T Averaged_image = average4to1(CVS_image);

        /* Compression Step #5: shit the bed */
        UArray2b_T DCT_image = CVS_to_DCT(Averaged_image);

        /*  */
        UArray2b_T Averaged_image2 = DCT_to_CVS(DCT_image);
        UArray2b_T CVS_image2 = average1to4(Averaged_image2);
        Pnm_ppm final_image = CompVidtoRGB(CVS_image2, methods, DENOMINATOR);
        // Pnm_ppmwrite(stdout, final_image);
        // if (Bitpack_fitsu(-21, 52)) {
        //         printf("FITS!\n");
        // }
        int64_t test_word = Bitpack_gets(0x3f4, 6, 2);
        printf("testword: %ld\n", test_word);

        Pnm_ppmfree(&ppm_image);
        UArray2b_free(&trimmed_image);
        UArray2b_free(&CVS_image);
        UArray2b_free(&CVS_image2);
        UArray2b_free(&Averaged_image);
        Pnm_ppmfree(&final_image);

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
        

        UArray2b_T CVS_image = methods->new_with_blocksize(0, 0, 
                                             sizeof(struct CompVidPixel *), 
                                             2);    
        
        Pnm_ppm ppm_image = CompVidtoRGB(CVS_image, methods, DENOMINATOR);
        Pnm_ppmwrite(stdout, ppm_image);  
}