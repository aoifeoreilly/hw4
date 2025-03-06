/**************************************************************
 *
 *                     read_write.c
 *
 *      Assignment: arith
 *      Authors:    Griffin Faecher (gfaech01) and Aoife O'Reilly (aoreil02)
 *      Date:       3/5/2025
 *
 *      Reads and writes a compressed binary image to and from standard output.
 *
 *************************************************************/

#include <stdio.h>
#include <assert.h>
#include <except.h>
#include "read_write.h"

/********** write_out ********
 *
 * Writes the given compressed binary image to standard output.
 *
 * Parameters:
 *      UArray2_T packed_image: The compressed image to write.
 *      int trimmed_width     : The width of the image to write.
 *      int trimmed_height    : The height of the image to write.
 * 
 * Return: 
 *      None.
 *
 * Expects:
 *     A valid 2D blocked array.
 * 
 * Notes:
 *      Prints the byte of the image to standard output in big endian order.
 *     
 ************************/
void write_out(UArray2_T packed_image, int trimmed_width, int trimmed_height)
{
        (void)packed_image;

        /* Write the header followed by a newline */
        printf("COMP40 Compressed image format 2\n%u %u", trimmed_width, 
                                                          trimmed_height);
        printf("\n");
        for (int row = 0; row < UArray2_height(packed_image); row++) {
                for (int col = 0; col < UArray2_width(packed_image); col++) {
                        uint32_t *word = UArray2_at(packed_image, col, row);
                        // for (int i = 0; i < 4; i++) {
                        //         putchar(Bitpack_getu(*word, 8, 24 - (i * 8)));
                        // }
                        /* Get each byte in the 32-bit code word */
                        uint64_t byte1 = Bitpack_getu(*word, 8, 24);
                        uint64_t byte2 = Bitpack_getu(*word, 8, 16);
                        uint64_t byte3 = Bitpack_getu(*word, 8, 8);
                        uint64_t byte4 = Bitpack_getu(*word, 8, 0);

                        char byte1_ch = (char)byte1;
                        char byte2_ch = (char)byte2;
                        char byte3_ch = (char)byte3;
                        char byte4_ch = (char)byte4;
                        
                         /* Write each code word in big endian (MSB first) */
                         printf("%c%c%c%c", byte1_ch, byte2_ch, 
                                            byte3_ch, byte4_ch);
                }
        }
        // printf("Counter: \n%d", counter);
}

/********** read_input ********
 *
 *  Reads in the compressed image of 32-bit code words using fscanf.
 *
 * Parameters:
 *      FILE *input: A FILE pointer to the image to read in.
 * 
 * Return: 
 *      A 2D blocked array containing the compressed image pixels.
 *
 * Expects:
 *     The given file not to be NULL.
 * 
 * Notes:
 *      Will C.R.E. if the given file is NULL.
 *      Allocates a new 2D blocked array of pixels.
 *      Use getc to read characters from the given file stream in big endian.
 * 
 ************************/
UArray2_T read_input(FILE *input)
{
        assert(input != NULL);
        
        /* Read the header of the compressed file using fscanf */
        unsigned height, width;
        int read = fscanf(input, "COMP40 Compressed image format 2\n%u %u", 
                          &width, &height);
        (void)read;
        assert(read == 2);
        int c = getc(input);
        assert(c == '\n');

        /* Allocate 2D array of pixels of the given width and height */
        UArray2_T packed_image = UArray2_new(width / BLOCKSIZE, 
                                             height / BLOCKSIZE, 
                                             sizeof(uint32_t));
        assert(packed_image != NULL);
        assert(UArray2_height(packed_image) % 2 == 0);
        assert(UArray2_width(packed_image) % 2 == 0);

        // printf("packed image height: %d\n", UArray2_height(packed_image));
        // printf("packed image width: %d\n", UArray2_width(packed_image));
        
        /* Place the array, width, height, and denominator in local variable */
        for (int row = 0; row < UArray2_height(packed_image); 
                                                                row++) {
                for (int col = 0; col < UArray2_width(packed_image);
                                                                col++) {
                        /* Get a pointer to each element in packed_image */
                        uint32_t *word = UArray2_at(packed_image, col, row);
                        
                        /* Use getc to read a single character from the given 
                        file stream in big endian order (MSB first) */
                        uint64_t byte1 = getc(input);
                        uint64_t byte2 = getc(input);
                        uint64_t byte3 = getc(input);
                        uint64_t byte4 = getc(input);    
                        *word = Bitpack_newu(*word, 8, 24, byte1);
                        *word = Bitpack_newu(*word, 8, 16, byte2);
                        *word = Bitpack_newu(*word, 8, 8, byte3);
                        *word = Bitpack_newu(*word, 8, 0, byte4);      
                }
        }
        return packed_image;
}