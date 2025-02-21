/**************************************************************
 *
 *                     ppmdiff.c
 *
 *     Assignment: locality
 *     Authors:    Griffin Faecher (gfaech01), Aoife O'Reilly (aoreil02)
 *     Date:       2/21/2025
 *
 *     This file implements the ppmdiff program.
 *
 *************************************************************/

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
void checkBorders(Pnm_ppm image1, Pnm_ppm image2, int *height, int *width, 
                                                        double *denominator);

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
 *      Command-line arguments to specify image transformations.
 *      A valid image file (or stdin) should be provided.
 *      Proper memory allocation and deallocation.
 *
 * Notes:
 
 *      Reads in PPM images.
 ************************/
int main(int argc, char *argv[])
{
        
        /* Default to UArray2 methods */
        A2Methods_T methods = uarray2_methods_plain;
        assert(methods != NULL);
        
        FILE *fptr1 = NULL;
        FILE *fptr2 = NULL;

        assert(argc > 1 && argc < 4);
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
        printf("RMS: %f\n", rms);
        
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
 *      char *fname:    A pointer to the name of the file to be opened.
 *      char *mode:     A pointer to the mode string.
 *
 * Return:
 *      FILE *:         A pointer to the opened file.
 *
 * Expects:
 *      The pointers fname and mode must not be NULL.
 *      The file must exist and be accessible with the given mode.
 *
 * Notes:
 *      Will CRE if any expectation is violated.
 ************************/
static FILE *openFile(char *fname, char *mode)
{
        assert(fname != NULL && mode != NULL);
        FILE *fp = fopen(fname, mode);
        assert(fp != NULL);
        return fp;
}

/********** find_rms ********
 *
 * 
 *
 * Parameters:
 *      
 *
 * Return:
 *      
 *
 * Expects:
 *      
 *
 * Notes:
 *      
 ************************/
double find_rms(Pnm_ppm image1, Pnm_ppm image2)
{
        int height = 0;
        int width = 0;
        double denominator = 0;
        checkBorders(image1, image2, &height, &width, &denominator);
        double sum = 0;
        for (int i = 0; i < height; i++) {
                for (int j = 0; j < width; j++) {
                        Pnm_rgb pixel1 = image1->methods->at(image1->pixels, 
                                                                        j, i);
                        Pnm_rgb pixel2 = image2->methods->at(image2->pixels, 
                                                                        j, i);
                        
                        double diff_red = ((double)pixel1->red - 
                                (double)pixel2->red) / denominator;

                        double diff_green = ((double)pixel1->green - 
                                (double)pixel2->green) / denominator;

                        double diff_blue = ((double)pixel1->blue - 
                                (double)pixel2->blue) / denominator;

                        
                        sum += (diff_red * diff_red) 
                             + (diff_green * diff_green) 
                             + (diff_blue * diff_blue);
                }
        }
        
        double mean = sum / (3 * width * height);
        double rms = sqrt(mean);

        return rms;
        
}


void checkBorders(Pnm_ppm image1, Pnm_ppm image2, int *height, int *width, 
                                                  double *denominator) 
{
        *denominator = image1->denominator;
        assert(*denominator == image2->denominator);
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
    

