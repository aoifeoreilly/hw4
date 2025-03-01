/**************************************************************
 *
 *                     unit_tests.c
 *
 *     Assignment: locality
 *     Authors:    Griffin Faecher (gfaech01) and Aoife O'Reilly (aoreil02)
 *     Date:       2/24/2025
 *
 *     This file contains unit testing for 
 *
 **************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <except.h>

#include "a2methods.h"
#include "a2blocked.h"
#include "uarray2b.h"
#include "ppmdiff.h"
#include "compress_decompress.h"
#include "trimPPM.h"
#include "bitpack.h"


/*****************************************************************
 *                      Compression Tests
 *****************************************************************/

void compress_valid_file_test()
{
        FILE *file = fopen("test_files/valid.ppm", "r");
        assert(file != NULL);

        TRY
        {
                compress40(file);
                printf("Success: %s\n", __func__);
        }
        EXCEPT(Assert_Failed)
        {
                printf("Failure: %s\n", __func__);
        }
        END_TRY;

        fclose(file);
}

void compress_null_file_test()
{
        TRY
        {
                compress40(NULL);
        }
        EXCEPT(Assert_Failed)
        {
                printf("Success: %s\n", __func__);
                return;
        }
        END_TRY;

        printf("Failure: %s\n", __func__);
}

void compress_invalid_file_test()
{
        FILE *file = fopen("test_files/invalid.ppm", "r");
        assert(file != NULL);

        TRY
        {
                compress40(file);
        }
        EXCEPT(Assert_Failed)
        {
                printf("Success: %s:\n", __func__);
                fclose(file);
                return;
        }
        END_TRY;

        printf("Failure: %s\n", __func__);
        fclose(file);
}

void trimPPM_test()
{
    FILE *input = fopen("test_files/odd_dim.ppm", "r");
    assert(input != NULL);

    A2Methods_T methods = uarray2_methods_blocked;
    assert(methods != NULL);

    /* Read the PPM image */
    Pnm_ppm ppm_image = Pnm_ppmread(input, methods);
    assert(ppm_image != NULL);
    fclose(input);

    A2Methods_T comp_vid_image = trimPPM(ppm_image);
    assert(comp_vid_image != NULL);

    /* Check that the dimensions are now even */
    int new_width = UArray2b_width(comp_vid_image);
    int new_height = UArray2b_height(comp_vid_image);
    assert(new_width % 2 == 0);
    assert(new_height % 2 == 0);

    printf("Success: %s\n", __func__);

    UArray2b_free(&comp_vid_image);
    Pnm_ppmfree(&ppm_image);
}

void rgbToCompVid_test()
{
        FILE *input = fopen("test_files/valid.ppm", "r");
        A2Methods_T methods = uarray2_methods_blocked;
        Pnm_ppm ppm_image = Pnm_ppmread(input, methods);
        assert(ppm_image != NULL);
        fclose(input);

        /* Create an array to hold the component video data */
        UArray2b_T comp_vid_image = UArray2b_new(ppm_image->width, 
                                                ppm_image->height, 
                                                sizeof(struct CompVidPixel), 2);
        assert(comp_vid_image != NULL);

        /* Perform the RGB to Component Video conversion */
        rgbToCompVid(comp_vid_image, ppm_image);

        /* Validate the results */
        for (unsigned row = 0; row < ppm_image->height; row++) {
                for (unsigned col = 0; col < ppm_image->width; col++) {
                        struct CompVidPixel *pixel = 
                                        UArray2b_at(comp_vid_image, col, row);
                        assert(pixel->Y >= 0.0 && pixel->Y <= 1.0);
                        assert(pixel->Pb >= -0.5 && pixel->Pb <= 0.5);
                        assert(pixel->Pr >= -0.5 && pixel->Pr <= 0.5);
                }
        }

        printf("Success: %s\n", __func__);

        UArray2b_free(&comp_vid_image);
        Pnm_ppmfree(&ppm_image);
}

/*****************************************************************
 *                      Decompression Tests
 *****************************************************************/



/*****************************************************************
 *                      Bitpack Tests
 *****************************************************************/


/*****************************************************************
 *                      Main
 *****************************************************************/
int main()
{
        compress_valid_file_test();
        compress_null_file_test();
        trimPPM_test();
        rgbToCompVid_test();
        return 0;
}