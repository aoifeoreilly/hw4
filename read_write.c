/**************************************************************
 *
 *                     read_write.c
 *
 *      Assignment: arith
 *      Authors:    Griffin Faecher (gfaech01) and Aoife O'Reilly (aoreil02)
 *      Date:       3/5/2025
 *
 *      This file
 *
 *************************************************************/

#include <stdio.h>
#include <assert.h>
#include <except.h>
#include "read_write.h"

/********** write ********
 *
 * 
 *
 * Parameters:
 *      UArray2b_T packed_image:
 * 
 * Return: 
 *
 * Expects:
 *     
 * 
 * Notes:
 *     
 ************************/
void write(UArray2b_T packed_image, int trimmed_width, int trimmed_height)
{
        (void)packed_image;

        /* Write the header followed by a newline */
        printf("COMP40 Compressed image format 2\n%u %u", trimmed_width, 
                                                          trimmed_height);
        printf("\n");

        for (int row = 0; row < UArray2b_height(packed_image); row++) {
                for (int col = 0; col < UArray2b_width(packed_image); col++) {
                        uint32_t *word = UArray2b_at(packed_image, col, row);
                        // for (int i = 0; i < 4; i++) {
                        //         putchar(Bitpack_getu(*word, 8, 24 - (i * 8)));
                        // }
                        /* Get each byte in the 32-bit code word */
                        unsigned byte1 = Bitpack_getu(*word, 8, 24);
                        signed byte2 = Bitpack_getu(*word, 8, 16);
                        signed byte3 = Bitpack_getu(*word, 8, 8);
                        signed byte4 = Bitpack_getu(*word, 8, 0);

                         /* Write each code word in big endian (MSB first) */
                         printf("%c%c%c%c", (char)byte1, (char)byte2, 
                                            (char)byte3, (char)byte4);
                }
        }
}

/********** read ********
 *
 * 
 *
 * Parameters:
 *      FILE *input:
 * 
 * Return: 
 *
 * Expects:
 *     
 * 
 * Notes:
 *     
 ************************/
UArray2b_T read_input(FILE *input)
{
        (void)input;
        // /* Read the header of the compressed file using fscanf */
        unsigned height, width;
        int read = fscanf(input, "COMP40 Compressed image format 2\n%u %u", &width, &height);
        (void)read;
        assert(read == 2);
        int c = getc(input);
        assert(c == '\n');

        /* Allocate a 2D array of pixels of the given width and height */
        UArray2b_T packed_image = UArray2b_new(width / BLOCKSIZE, 
                                               height / BLOCKSIZE, 
                                               sizeof(uint32_t),
                                               BLOCKSIZE);
        assert(packed_image != NULL);

        /* Place the array, width, height, and denominator in local variable */
        for (int row = 0; row < UArray2b_height(packed_image) / BLOCKSIZE; row++) {
                for (int col = 0; col < UArray2b_width(packed_image) / BLOCKSIZE; col++) {
                        /* Get a pointer to each element in packed_image */
                        uint32_t *word = UArray2b_at(packed_image, col, row);
                        (void)word;
                        /* Use getc to read a single character from the given 
                        file stream in big endian order (MSB first) */
                        int byte1 = getc(input);
                        int byte2 = getc(input);
                        int byte3 = getc(input);
                        int byte4 = getc(input);    

                        *word = Bitpack_newu(*word, 8, 24, byte1);
                        *word = Bitpack_newu(*word, 8, 16, byte2);
                        *word = Bitpack_newu(*word, 8, 8, byte3);
                        *word = Bitpack_newu(*word, 8, 0, byte4);      
                }
        }
        return packed_image;
}