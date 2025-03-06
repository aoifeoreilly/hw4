/*************************************************************
 *
 *                   compress_decompress.c
 *
 *      Assignment: arith
 *      Edited by:  Aoife O'Reilly (aoreil02) and Griffin Faecher (gfaech01)
 *      Date:       2/28/2025
 *
 *      Carries out the compression and decompression algorithms as outlined 
 *      in the spec. This program converts the pixels within a given ppm image
 *      to a smaller binary image consisting of 32-bit codewords and back to 
 *      carry out the image manipulation process.
 *
 **********************************************************/

#include "compress_decompress.h"
#define DENOMINATOR 255

/********** compression ********
 *
 * Compresses a full-color portable pixmap to a smaller binary image.
 *
 * Parameters:
 *      FILE *input: A file pointer to the image to compress.
 * 
 * Return:
 *      None.
 *
 * Expects:
 *      The given file, method, and map function not to be NULL.
 *
 * Notes:
 *      Writes the compressed image to standard output.
 *      Will C.R.E. if the given file, method, or map function are NULL.
 *      Will C.R.E if any of the non-void functions return a NULL argument.
 *     
 ************************/
void compress40(FILE *input)
{
        /* Verify valid input file and use UArray2b blocked methods */
        assert(input != NULL);
        A2Methods_T methods = uarray2_methods_blocked;
        assert(methods != NULL);
        A2Methods_mapfun *map = methods->map_default; 
        assert(map != NULL);
        
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
        UArray2b_T averaged_image = average4to1(CVS_image);
        assert(averaged_image != NULL);
        UArray2b_free(&CVS_image);

        /* Compression Step #5: Use DCT to transform to cosine coeffecients */
        UArray2b_T DCT_image = CVS_to_DCT(averaged_image);
        assert(DCT_image != NULL);
        UArray2b_free(&averaged_image);

        /* Compression Step #6: Pack a, b, c, d, PB, and PR into 32-bit word */
        UArray2_T packed_image = pack_image(DCT_image);
        assert(packed_image != NULL);
        UArray2b_free(&DCT_image);

        /* Compression Step #7: Write compressed image to standard output */
        write_out(packed_image, trimmed_width, trimmed_height);
        UArray2_free(&packed_image);
}

/********** decompression ********
 *
 * Decompresses a binary image into a full-color portable pixmap.
 *
 * Parameters:
 *      FILE *input:  A file pointer to the image to decompress.
 * 
 * Return:
 *      None.
 *
 * Expects:
 *      The given file, method, and map function not to be NULL.
 *
 * Notes:
 *      Writes the decompressed image to standard output.
 *      Will C.R.E. if the given file, method, or map function are NULL.
 *      Will C.R.E if any of the non-void functions return a NULL argument.
 * 
 ************************/
void decompress40(FILE *input)
{      
        assert(input != NULL);
        
        /* Decompression Step #1: Read the header of the compressed file */
        UArray2_T packed_image = read_input(input);

        /* Decompression Step #2: For each code word, unpack a, b, c, d, PB, 
                                  and PR into local variables */
        UArray2b_T DCT_image = unpack_image(packed_image);
        assert(DCT_image != NULL);
        UArray2_free(&packed_image);

        /* Decompression Step #4: Use inverse DCT to compute Y1, Y2, Y3, Y4 */
        UArray2b_T averaged_image = DCT_to_CVS(DCT_image);
        assert(averaged_image != NULL);
        UArray2b_free(&DCT_image);

        /* Decompression Step #5: Expand each pixel to 2-by-2 block and quantize
           RGB values to integers in a range corresponding to our denominator */
        UArray2b_T CVS_image = average1to4(averaged_image);
        assert(CVS_image != NULL);
        UArray2b_free(&averaged_image);

        /* Decompression Step #6: Transform CVS to RGB */
        A2Methods_T methods = uarray2_methods_blocked;
        assert(methods != NULL);
        A2Methods_mapfun *map = methods->map_default; 
        assert(map != NULL);

        Pnm_ppm ppm_image = CompVidtoRGB(CVS_image, methods, DENOMINATOR);
        assert(ppm_image != NULL);
        UArray2b_free(&CVS_image);

        /* Decompression Step #7: Write uncompressed image to standard output */
        Pnm_ppmwrite(stdout, ppm_image);
        Pnm_ppmfree(&ppm_image);
}