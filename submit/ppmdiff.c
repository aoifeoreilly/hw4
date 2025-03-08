/**************************************************************
 *
 *                     ppmdiff.c
 *
 *      Assignment: arith
 *      Authors:    Griffin Faecher (gfaech01) and Aoife O'Reilly (aoreil02)
 *      Date:       2/21/2025
 *
 *      This file implements the ppmdiff program. This program compares two 
 *      images to get a quantitative estimate of how similar they are. Helps
 *      to test the compressor and decompressor.
 *
 *************************************************************/

#include "ppmdiff.h"

/********** main ********
 *
 * Parses command-line arguments and processes an image accordingly.
 *
 * Parameters:
 *      int argc:     The number of command-line arguments.
 *      char *argv[]: The array of command-line argument strings.
 *
 * Return:
 *      int: Returns EXIT_SUCCESS (0) on successful execution.
 *
 * Expects:
 *      Command-line arguments to be PPM files. One of the arguments
 *      (but not both) may be given by standard input using "-".
 *      A valid image file (or stdin) should be provided.
 *      Proper memory allocation and deallocation.
 *
 * Notes:
 *      Reads in PPM images.
 * 
 ************************/
int main(int argc, char *argv[])
{
        
        /* Default to UArray2 methods */
        A2Methods_T methods = uarray2_methods_plain;
        assert(methods != NULL);
        
        FILE *fptr1 = NULL;
        FILE *fptr2 = NULL;

        assert(argc == 3);
        assert(strcmp(argv[1], "-") != 0 || strcmp(argv[2], "-") != 0);

        /* First argument to be read on stdin */
        if (strcmp(argv[1], "-") == 0) {
                fptr1 = stdin;
                fptr2 = openFile(argv[2], "r");
                
        /* Second argument to be read on stdin */
        } else if (strcmp(argv[2], "-") == 0) {
                fptr1 = openFile(argv[1], "r");
                fptr2 = stdin;

        /* Neither argument to be read on stdin */
        } else {
                fptr1 = openFile(argv[1], "r");
                fptr2 = openFile(argv[2], "r");
        }
        
        Pnm_ppm ppm1 = Pnm_ppmread(fptr1, methods);
        Pnm_ppm ppm2 = Pnm_ppmread(fptr2, methods);
        double rms = find_rms(ppm1, ppm2);
        printf("RMS: %.4f\n", rms);
        
        Pnm_ppmfree(&ppm1);
        Pnm_ppmfree(&ppm2);
        fclose(fptr1);
        fclose(fptr2);
        exit(EXIT_SUCCESS);
}

/********** openFile ********
 *
 * Opens a file with the given filename and mode.
 *
 * Parameters:
 *      char *fname:   A pointer to the name of the file to be opened.
 *      char *mode:    A pointer to the mode string.
 *
 * Return:
 *      FILE *:        A pointer to the opened file.
 *
 * Expects:
 *      The pointers fname and mode must not be NULL.
 *      The file must exist and be accessible with the given mode.
 *
 * Notes:
 *      Will CRE if any expectation is violated.
 ************************/
FILE *openFile(char *fname, char *mode)
{
        assert(fname != NULL && mode != NULL);
        FILE *fp = fopen(fname, mode);
        assert(fp != NULL);
        return fp;
}

/********** find_rms ********
 *
 * Calculates the root mean square difference of the pixel values in the two
 * given images.
 *
 * Parameters:
 *      Pnm_ppm image1: a PPM image
 *      Pnm_ppm image2: a PPM image
 *
 * Return:
 *      Double:      A single number to standard output which represents a
 *                   measure of the difference between the two input images.
 *
 * Expects:
 *      Two valid images to be provided.
 *
 * Notes:
 *      Converts the numerators and denominator to floating point values.
 *      Print the result to standard output with four digits after the decimal
 *      point.
 *      
 ************************/
double find_rms(Pnm_ppm image1, Pnm_ppm image2)
{
        int height = 0;
        int  width = 0;
        double sum = 0;
        
        double denominator = image1->denominator;
        assert(denominator != 0 && denominator == image2->denominator);
        checkBorders(image1, image2, &height, &width);
        
        for (int i = 0; i < height; i++) {
                for (int j = 0; j < width; j++) {
                        Pnm_rgb pixel1 = image1->methods->at(image1->pixels, 
                                                                        j, i);
                        Pnm_rgb pixel2 = image2->methods->at(image2->pixels, 
                                                                        j, i);
                        
                        sum += findSum(pixel1, pixel2, denominator);
                }
        }
        
        double mean = sum / (3 * width * height);
        double rms = sqrt(mean);

        return rms;
}

/********** checkBorders ********
 *
 * Verifies that the width and height of the two given images differ at most
 * by 1.
 *
 * Parameters:
 *      Pnm_ppm image1: a PPM image
 *      Pnm_ppm image2: a PPM image
 *      int *height:    a pointer to be updated to point to the height of the 
 *                      shorter image
 *      int *width:     a pointer to be updated to point to the width of the 
 *                      shorter image
 *
 * Return:
 *      Double:         A single number to standard output which represents a
 *                      measure of the difference between the two input images.
 *
 * Expects:
 *      Two valid images to be provided.
 *
 * Notes:
 *      Prints an error message to standard error and prints the number 1.0
 *      to standard output if the heights are of the two images differ by
 *      more than 1.
 * 
 ************************/
void checkBorders(Pnm_ppm image1, Pnm_ppm image2, int *height, int *width) 
{

        /* Check height */
        if (abs((int)image1->height - (int)image2->height) > 1) {
                fprintf(stderr, "HEIGHTS ARE DIFFERENT\n");
                printf("1.0\n");
                exit(1);
        } else {
                if(image1->height > image2->height) {
                        *height = image2->height;
                } else {
                        *height = image1->height;
                }   
        }

        /* Check width */
        if (abs((int)image1->width - (int)image2->width) > 1) {
                fprintf(stderr, "WIDTHS ARE DIFFERENT\n");
                printf("1.0\n");
                exit(1);
        } else {
                if (image1->width > image2->width) {
                        *width = image2->width;
                } else {
                        *width = image1->width;
                }   
        }
}
    
/********** findSum ********
 *
 * Sums the RGB values for the given pixels.
 *
 * Parameters:
 *      Pnm_rgb pixel1:         a pixel.
 *      Pnm_rgb pixel2:         a pixel.
 *      double denominator:     a double representing the maximum value that
 *                              the RGB values can go to.
 *
 * Return:
 *      Double: Sum of the RGB values of the given pixel.
 *
 * Expects:
 *      Two valid pixels to be provided.
 *      Nonzero denominator value.
 *
 * Notes:
 *      Casts the unsigned values as doubles.
 * 
 ************************/
double findSum(Pnm_rgb pixel1, Pnm_rgb pixel2, double denominator) 
{
        double diff_red = ((double)pixel1->red - (double)pixel2->red) 
                          / denominator;

        double diff_green = ((double)pixel1->green - (double)pixel2->green) 
                          / denominator;

        double diff_blue = ((double)pixel1->blue - (double)pixel2->blue) 
                          / denominator;

        
        return (diff_red * diff_red) + (diff_green * diff_green) 
                                     + (diff_blue * diff_blue);

}