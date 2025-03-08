/*************************************************************
 *
 *                     40image.c
 *
 *      Assignment: arith
 *      Edited by:  Aoife O'Reilly (aoreil02) and Griffin Faecher (gfaech01)
 *      Date:       2/28/2025
 *
 *      This file compresses or decompresses and image given command line
 *      arguments. Converts the given image from a full-color portable pixmap 
 *      to a compressed binary image file or decompresses it the other way.
 *
 **********************************************************/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "assert.h"
#include "compress_decompress.h"

/*      Function pointer that points to either compress40 or decompress40
        Returns a void pointer that points to the specified function    
*/
static void (*compress_or_decompress)(FILE *input) = compress40;

/********** main ********
 *
 * Parses command-line arguments and processes an image accordingly.
 *
 * Parameters:
 *      int argc:     The number of command-line arguments.
 *      char *argv[]: The array of command-line argument strings.
 *
 * Return:
 *      int: Returns EXIT_SUCCESS (0) on successful execution, if not 
 *           EXIT_FAILURE (1).
 *
 * Expects:
 *      Command-line argument to be PPM files.
 *
 * Notes:
 *      Reads in PPM images.
 *      Prints an error message to stderr if there is an unknown 
 *      option after "-" or improper format of arguments on the command line.
 *      Throws a checked runtime error if there more than one file is given 
 *      on the command line.
 ************************/
int main(int argc, char *argv[])
{
        int i;
        for (i = 1; i < argc; i++) {
                if (strcmp(argv[i], "-c") == 0) {
                        compress_or_decompress = compress40;
                } else if (strcmp(argv[i], "-d") == 0) {
                        compress_or_decompress = decompress40;
                } else if (*argv[i] == '-') {
                        fprintf(stderr, "%s: unknown option '%s'\n",
                                argv[0], argv[i]);
                        exit(1);
                } else if (argc - i > 2) {
                        fprintf(stderr, "Usage: %s -d [filename]\n"
                                "       %s -c [filename]\n",
                                argv[0], argv[0]);
                        exit(1);
                } else {
                        break;
                }
        }
        assert(argc - i <= 1);    /* at most one file on command line */
        if (i < argc) {
                FILE *fp = fopen(argv[i], "r");
                assert(fp != NULL);
                compress_or_decompress(fp);
                fclose(fp);
        } else {
                compress_or_decompress(stdin);
        }

        return EXIT_SUCCESS; 
}
