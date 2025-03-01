/*************************************************************
 *
 *                 convert_comp_rgb.c
 *
 *      Assignment: arith
 *      Edited by:  Aoife O'Reilly (aoreil02) and Griffin Faecher (gfaech01)
 *      Date:       2/28/2025
 *
 *      
 *
 **********************************************************/

#include "convert_comp_rgb.h"

struct CompVidClosure {
        UArray2b_T comp_vid_image;
        int denominator;
};

struct AverageCompVidPixel {
        float Y1, Y2, Y3, Y4;
        float Pb_avg, Pr_avg;
};

struct RGBClosure {
        UArray2b_T comp_vid_image;
        int denominator;
};

void rgbToCompVid(UArray2b_T comp_vid_image, Pnm_ppm ppm_image) 
{
        struct CompVidClosure cl = {
                comp_vid_image, 
                ppm_image->denominator
        };

        ppm_image->methods->map_default(ppm_image->pixels, rgbToCompVidApply, 
                                        &cl);
                                        
        (void)comp_vid_image;
        (void)ppm_image;

}

void rgbToCompVidApply(int col, int row, A2Methods_Object *array2b, void *elem, 
                       void *cl)
{
        (void)array2b;
        
        Pnm_rgb rgb_pixel = elem;
        struct CompVidClosure *CV_cl = cl;
        UArray2b_T comp_vid_image = CV_cl->comp_vid_image;
        int denominator = CV_cl->denominator;

        /* Make a component video pixel instance from our new image */
        struct CompVidPixel *comp_vid_pixel = UArray2b_at(comp_vid_image, 
                                                          col, row);
        
        float red_float = rgb_pixel->red / denominator;
        float green_float = rgb_pixel->green / denominator;
        float blue_float = rgb_pixel->blue / denominator;

        /* Calculate values for Y, Pb, and Pr */
        comp_vid_pixel->Y = (0.299 * red_float) + 
                            (0.587 * green_float) + 
                            (0.114 * blue_float);
        comp_vid_pixel->Pb = (-0.168736 * red_float) - 
                             (0.331264 * green_float) + 
                             (0.5 * blue_float);
        comp_vid_pixel->Pr = (0.5 * red_float) - 
                             (0.418688 * green_float) - 
                             (0.081312 * blue_float);

        /* QUESTION: what happens if these values are out of range, what should
        we do? */
        assert(comp_vid_pixel->Y <= 1.0 &&  comp_vid_pixel->Y >= 0.0);
        assert(comp_vid_pixel->Pb <= 0.5 &&  comp_vid_pixel->Pb >= -0.5);
        assert(comp_vid_pixel->Pr <= 0.5 &&  comp_vid_pixel->Pr >= -0.5);

        /* TODO: add logic to get Y1, Y2, Y3, Y4 */
}

void CompVidtoRGB(UArray2b_T comp_vid_image, Pnm_ppm ppm_image)
{
        // struct CompVidClosure cl = {
        //         ppm_image, 
        //         ppm_image->denominator
        // };


        
        ppm_image->methods->map_default(ppm_image->pixels, CompVidtoRGBApply, 
                                        &ppm_image);


} 

void CompVidtoRGBApply(int col, int row, A2Methods_Object *array2b, void *elem, 
                       void *cl)
{
        (void)array2b;
        struct AverageCompVidPixel *avg_comp_vid_pixel= elem;
        struct Pnm_ppm *ppm_image = cl;

        float Y1_float = avg_comp_vid_pixel->Y1;
        float Y2_float = avg_comp_vid_pixel->Y2;
        float Y3_float = avg_comp_vid_pixel->Y3;
        float Y4_float = avg_comp_vid_pixel->Y4;
        float Pb_float = avg_comp_vid_pixel->Pb;
        float Pr_float = avg_comp_vid_pixel->Pr;

        unsigned r1 = (unsigned)(((1.0 * Y1_float) + 
                                  (0.0 * Pb_float) + 
                                  (1.402 * Pr_float)) *
                                  ppm_image->denominator);
        unsigned g1 = (unsigned)(((1.0 * Y1_float) + 
                                  (0.344136 * Pb_float) + 
                                  (0.714136 * Pr_float)) *
                                  ppm_image->denominator);
        unsigned b1 = (unsigned)(((1.0 * Y1_float) + 
                                  (1.772 * Pb_float) + 
                                  (0.0 * Pr_float)) *
                                  ppm_image->denominator);

        unsigned r2 = (unsigned)(((1.0 * Y2_float) + 
                                  (0.0 * Pb_float) + 
                                  (1.402 * Pr_float)) *
                                  ppm_image->denominator);
        unsigned g2 = (unsigned)(((1.0 * Y2_float) + 
                                  (0.344136 * Pb_float) + 
                                  (0.714136 * Pr_float)) *
                                  ppm_image->denominator);
        unsigned b2 = (unsigned)(((1.0 * Y2_float) + 
                                  (1.772 * Pb_float) + 
                                  (0.0 * Pr_float)) *
                                  ppm_image->denominator);

        unsigned r3 = (unsigned)(((1.0 * Y3_float) + 
                                  (0.0 * Pb_float) + 
                                  (1.402 * Pr_float)) *
                                  ppm_image->denominator);
        unsigned g3 = (unsigned)(((1.0 * Y3_float) + 
                                  (0.344136 * Pb_float) + 
                                  (0.714136 * Pr_float)) *
                                  ppm_image->denominator);
        unsigned b3 = (unsigned)(((1.0 * Y3_float) + 
                                  (1.772 * Pb_float) + 
                                  (0.0 * Pr_float)) *
                                  ppm_image->denominator);

        unsigned r4 = (unsigned)(((1.0 * Y4_float) + 
                                  (0.0 * Pb_float) + 
                                  (1.402 * Pr_float)) * 
                                  ppm_image->denominator);
        unsigned g4 = (unsigned)(((1.0 * Y4_float) - 
                                  (0.344136 * Pb_float) -
                                  (0.714136 * Pr_float)) *
                                  ppm_image->denominator);
        unsigned b4 = (unsigned)(((1.0 * Y4_float) + 
                                  (1.772 * Pb_float) + 
                                  (0.0 * Pr_float)) *
                                  ppm_image->denominator);
        /* QUESTION: How to take average image data spread across 4 pixel in ppm
        */
        for (int i = 0; i < ppm_image->methods->blocksize(ppm_image->pixels); i++) {
                for (int j = 0; j < ppm_image->methods->blocksize(ppm_image->pixels); j++) {
                        Pnm_rgb rgb_pixel = ppm_image->methods->at(ppm_image->pixels, 
                                                                   col + i, row + j);
                        rgb_pixel->red = (unsigned)(((1.0 * Y1_float) + 
                                  (0.0 * Pb_float) + 
                                  (1.402 * Pr_float)) *
                                  ppm_image->denominator);
                        rgb_pixel->green = (unsigned)(((1.0 * Y1_float) + 
                                  (0.344136 * Pb_float) + 
                                  (0.714136 * Pr_float)) *
                                  ppm_image->denominator);
                        rgb_pixel->blue = (unsigned)(((1.0 * Y1_float) + 
                                  (1.772 * Pb_float) + 
                                  (0.0 * Pr_float)) *
                                  ppm_image->denominator);

                }
        }
}