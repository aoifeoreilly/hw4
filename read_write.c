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

        /* Write the header followed by a newline */
        printf("COMP40 Compressed image format 2\n%u %u", trimmed_width, 
                                                          trimmed_height);
        printf("\n");

        UArray2_map_row_major(packed_image, write_out_apply, NULL);
}

/********** write_out_apply ********
 *
 * Apply function to help map through the 2D blocked array and write each 
 * compressed image pixel to standard output.
 *
 * Parameters:
 *      int col   : The current column in the array of code words.
 *      int row   : The current row in the array of code words.
 *      UArray2_T packed_image: The 2D blocked array of compressed image pixels.
 *      void *elem: Pointer to each pixel in the 2D blocked array.
 *      void *cl  : Unused.
 * 
 * Return: 
 *      None.
 *
 * Expects:
 *      A valid UArray2_T array.
 *      Properly initialized element and closure pointers.
 * 
 * Notes:
 *      Will C.R.E. if the pointer arguments are passed in as NULL.
 * 
 ************************/

void write_out_apply(int col, int row, UArray2_T packed_image, void *elem, 
                                                                void *cl)
{
        (void)col;
        (void)row;
        (void)packed_image;
        (void)cl;
        uint32_t *word = elem;
        /* Get each byte in the 32-bit code word */
        uint32_t byte1 = (uint32_t)Bitpack_getu(*word, 8, 24);
        uint32_t byte2 = (uint32_t)Bitpack_getu(*word, 8, 16);
        uint32_t byte3 = (uint32_t)Bitpack_getu(*word, 8, 8);
        uint32_t byte4 = (uint32_t)Bitpack_getu(*word, 8, 0);

        char byte1_ch = (char)byte1;
        char byte2_ch = (char)byte2;
        char byte3_ch = (char)byte3;
        char byte4_ch = (char)byte4;
        
        /* Write each code word in big endian (MSB first) */
        printf("%c%c%c%c", byte1_ch, byte2_ch, byte3_ch, byte4_ch);
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
        UArray2_map_row_major(packed_image, read_input_apply, input);

        return packed_image;
}

/********** read_input_apply ********
 *
 * Apply function to help map through the 2D blocked array and read in each 
 * compressed image pixel from standard input.
 *
 * Parameters:
 *      int col   : The current column in the array of code words.
 *      int row   : The current row in the array of code words.
 *      UArray2_T packed_image: The 2D blocked array of compressed image pixels.
 *      void *elem: Pointer to each pixel in the 2D blocked array.
 *      void *cl  : Pointer to the file stream to read in the compressed image.
 * 
 * Return: 
 *      None.
 *
 * Expects:
 *      A valid UArray2_T array.
 *      Properly initialized element and closure pointers.
 * 
 * Notes:
 *      Will C.R.E. if the pointer arguments are passed in as NULL.
 * 
 ************************/
void read_input_apply(int col, int row, UArray2_T packed_image, void *elem, 
                                                                void *cl)
{
        (void)col;
        (void)row;
        (void)packed_image;
        FILE *input = cl;
        assert(input != NULL);
        
        /* Get a pointer to each element in packed_image */
        uint32_t *word = elem;
        
        /* Use getc to read a single character from the given file stream in 
        big endian order (MSB first) */
        uint64_t byte1 = getc(input);
        uint64_t byte2 = getc(input);
        uint64_t byte3 = getc(input);
        uint64_t byte4 = getc(input);    
        *word = Bitpack_newu(*word, 8, 24, byte1);
        *word = Bitpack_newu(*word, 8, 16, byte2);
        *word = Bitpack_newu(*word, 8, 8, byte3);
        *word = Bitpack_newu(*word, 8, 0, byte4);
}