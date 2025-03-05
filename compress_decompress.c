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
        /* Verify valid input file and use UArray2 blocked methods */
        assert(input != NULL);
        A2Methods_T methods = uarray2_methods_blocked;
        assert(methods != NULL);
         
        /* Compression Step #1: Read in the ppm image */
        Pnm_ppm ppm_image = Pnm_ppmread(input, methods);
        assert(ppm_image != NULL);
        unsigned denominator = ppm_image->denominator;
        
        /* Compression Step #2: Allocate UArray2b_T for image with trimmed 
                                dimensions in component video color space */
        UArray2b_T trimmed_image = trimPPM(ppm_image);
        assert(trimmed_image != NULL);
        Pnm_ppmfree(&ppm_image);

        /* Compression Step #3: Convert RGB values to comp video color format */
        UArray2b_T CVS_image = rgbToCompVid(trimmed_image, denominator);
        assert(CVS_image != NULL);
        int trimmed_width = UArray2b_width(trimmed_image);
        int trimmed_height = UArray2b_height(trimmed_image);
        UArray2b_free(&trimmed_image);

        /* Compression Step #4: Average value the chroma elements in every 
           four pixel block and convert to 4-bit quantized representation */
        UArray2b_T Averaged_image = average4to1(CVS_image);
        assert(Averaged_image != NULL);
        UArray2b_free(&CVS_image);

        /* Compression Step #5: Use DCT to transform to cosine coeffecients */
        UArray2b_T DCT_image = CVS_to_DCT(Averaged_image);
        assert(DCT_image != NULL);

        /* Compression Step #6: Pack a, b, c, d, PB, and PR into 32-bit word 
                       and write compressed image to standard output */
        printf("COMP40 Compressed image format 2\n%u %u", trimmed_width, trimmed_height);
        printf("\n");
        int width = UArray2b_width(DCT_image);
        int height = UArray2b_height(DCT_image);

        for (int row = 0; row < height; row++) {
                for (int col = 0; col < width; col++) {
                        uint32_t *word = UArray2b_at(DCT_image, col, row);
                        (void)word;
                }
        }

        /* Decompression Steps */
        UArray2b_T Averaged_image2 = DCT_to_CVS(DCT_image);
        UArray2b_T CVS_image2 = average1to4(Averaged_image2);
        Pnm_ppm final_image = CompVidtoRGB(CVS_image2, methods, DENOMINATOR);
        // Pnm_ppmwrite(stdout, final_image);
        // if (Bitpack_fitsu(-21, 52)) {
        //         printf("FITS!\n");
        // }
        int64_t test_word = Bitpack_gets(0x3f4, 6, 2);
        printf("testword: %ld\n", test_word);


        /* Free all allocated memory */
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
        
        /* Decompression Step #1: Read the header of the compressed file */
        unsigned height, width;
        int read = fscanf(input, "COMP40 Compressed image format 2\n%u %u", 
                          &width, &height);
        assert(read == 2);
        int c = getc(input);
        assert(c == '\n');

        /* Decompression Step #2: Allocate 2D array of pixels */
        UArray2b_T DCT_image = UArray2b_new(width / 2, height / 2, 
                                           sizeof(uint32_t), 2);    
        
        /* Decompression Step #4: Use inverse DCT to compute Y1, Y2, Y3, Y4 */
        UArray2b_T Averaged_image2 = DCT_to_CVS(DCT_image);

        /* Decompression Step #5: Expand each pixel to 2-by-2 block and quantize
           RGB values to integers in a range corresponding to our denominator */
        UArray2b_T CVS_image = average1to4(Averaged_image2);

        /* Decompression Step #6: Transform CVS to RGB and write the 
                                  uncompressed image to standard output */
        A2Methods_T methods = uarray2_methods_blocked;
        assert(methods != NULL);
        A2Methods_mapfun *map = methods->map_default; 
        assert(map != NULL);

        Pnm_ppm ppm_image = CompVidtoRGB(CVS_image, methods, DENOMINATOR);
        Pnm_ppmwrite(stdout, ppm_image);

        Pnm_ppmfree(&ppm_image);
        UArray2b_free(&DCT_image);
        UArray2b_free(&Averaged_image2);
        UArray2b_free(&CVS_image);
}