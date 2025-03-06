/**************************************************************
 *
 *                     bitpack.c
 *
 *      Assignment: arith
 *      Authors:    Griffin Faecher (gfaech01) and Aoife O'Reilly (aoreil02)
 *      Date:       3/5/2025
 *
 *      This interface manipulates bit fields with a 64 bit (8 byte) value 
 *      word. The provided functions use Hanson assertions to ensure that 
 *      shift values are for 64 bits or less, that widths are 64 or less, and
 *      that bit fields to be accessed or updated fit entirely within the 
 *      supplied 64 bit word.
 *
 *************************************************************/

#include <bitpack.h>
#include <stdio.h>
#include <assert.h>
#include <except.h>
#define LONG_1 0x0000000000000001
#define LONG_64 0x0000000000000040

Except_T Bitpack_Overflow = { "Overflow packing bits" };


/********** Bitpack_fitsu ********
 *
 * Determines whether the unsigned argument n can be represented in width bits.
 *
 * Parameters:
 *      uint64_t n    : an unsigned integer to check if it can be represented.
 *      unsigned width: the maximum number of bits that given argument can be
 *                      represent in.
 * 
 * Return: True if the argument 'n' can be represented in 'width' bits.
 *         False if the argument 'n' cannot be represented in 'width' bits.
 *
 * Expects:
 *      The width parameter to be less than or equal to 64.
 * 
 * Notes:
 *      Throws a C.R.E if the width parameter is greater than 64.
 *     
 ************************/
bool Bitpack_fitsu(uint64_t n, unsigned width)
{
        /* Width must be less than 64-bits */
        assert(width <= 64u);

        if (width == 0) {
                return false;
        }
        if (width == 64u) {
                return true;
        }
        uint64_t max = LONG_1;
        uint64_t shift = (max << width) - LONG_1;
        return n <= shift;
}

/********** Bitpack_fitss ********
 *
 * Determines whether the signed argument n can be represented in width bits.
 *
 * Parameters:
 *      int64_t n     : a signed integer to check if it can be represented.
 *      unsigned width: the maximum number of bits that given argument can be
 *                      represent in.
 * 
 * Return: True if the argument 'n' can be represented in 'width' bits.
 *         False if the argument 'n' cannot be represented in 'width' bits.
 *
 * Expects:
 *      The width parameter to be less than or equal to 64.
 *      The sum of the width and lsb paramter to be less than or equal to 64.
 * 
 * Notes:
 *      Throws a C.R.E if the width parameter is greater than 64.
 *      Throws a C.R.E if the sum of the width and lsb parametes is greater 
 *      than 64.
 ************************/
bool Bitpack_fitss(int64_t n, unsigned width)
{
        /* Width must be less than 64-bits */
        assert(width <= 64u);

        /* Width cannot be 0 bits. If width is 64 bits, all data should fit */
        if (width == 0u) {
                return false;
        }
        if (width == 64u) {
                return true;
        }
        /* Find the min and max values given width, and check to see if n fits*/
        int64_t max = (LONG_1 << (width - LONG_1)) - LONG_1;
        int64_t min = ~max;
        return n <= max && n >= min;
}

/********** Bitpack_getu ********
 *
 * Extracts a unsigned value from a word given the width of the field and the  
 * location of the unsigned value's least significant bit.
 *
 * Parameters:
 *      uint64_t word:  the codeword to have the value extracted from.
 *      unsigned width: the amount of bits of the extracted value.
 *      unsigned lsb:   the first bit in the word to begin extracting from.
 * 
 * Return: 
 *      A 64-bit unsigned integer extracted value from the given codeword.
 *
 * Expects:
 *      Width to be greater than or equal to 0, and less than or equal to 64.
 * 
 * Notes:
 *      Throws a C.R.E if called with width that is not 0 <= w <= 64.
 * 
 ************************/
uint64_t Bitpack_getu(uint64_t word, unsigned width, unsigned lsb)
{
        /* Width and sum of width and lsb both must be less than 64-bits */
        assert(width + lsb <= LONG_64);
        assert(width <= LONG_64);

        /* If width is zero, there is no value to get */
        if (width == 0) {
                return 0;
        }

        /* Shift the mask to the correct position in the word */
        uint64_t mask = ((LONG_1 << width) - LONG_1) << lsb;

        /* Shift back to lsb and return the bits specified by user */
        return (word & mask) >> lsb;
}

/********** Bitpack_gets ********
 *
 * Extracts a signed value from a word given the width of the field and the  
 * location of the signed value's least significant bit.
 *
 * Parameters:
 *      uint64_t word:  the codeword to have the value extracted from.
 *      unsigned width: the amount of bits of the extracted value.
 *      unsigned lsb:   the first bit in the word to begin extracting from.
 * 
 * Return: 
 *      A 64-bit signed integer extracted value from the given codeword.
 *
 * Expects:
 *      Width to be greater than or equal to 0, and less than or equal to 64.
 * 
 * Notes:
 *      Throws a C.R.E if called with width that is not 0 <= w <= 64.
 * 
 ************************/
int64_t Bitpack_gets(uint64_t word, unsigned width, unsigned lsb)
{
        /* Width and sum of width and lsb both must be less than 64-bits */
        assert(width + lsb <= LONG_64);
        assert(width <= LONG_64);

        /* If width is zero, there is no value to get */
        if (width == 0) {
                return 0;
        }
        /* Position mask in correct space in the word */
        uint64_t mask = ((LONG_1 << width) - LONG_1) << lsb;
        
        /* Find bits specified by user and shift them to lsb of 64 bit word */
        int64_t value = (word & mask) >> lsb;
        
        /* If the number is signed, extend the sign */
        if (value & (LONG_1 << (width - LONG_1))) {
                value = value | ~(mask >> lsb);
        }

        return value;

}

/********** Bitpack_newu ********
 *
 * Creates a new word which is identical to the original word, but the field 
 * of the given width and lsb will be replaced by a width-bit representation
 * of value.
 *
 * Parameters:
 *      uint64_t word:  the original codeword.
 *      unsigned width: the amount of bits for the updated field.
 *      unsigned lsb:   the least significant bit of the field.
 *      uint64_t value: the given unsigned int that should fit in the width,
 * 
 * Return: 
 *      A 64-bit unsigned integer codeword.
 *
 * Expects:
 *      Width to be greater than or equal to 0, and less than or equal to 64.
 * 
 * Notes:
 *      Throws a C.R.E if called with width that is not 0 <= w <= 64.
 * 
 ************************/
uint64_t Bitpack_newu(uint64_t word, unsigned width, unsigned lsb, 
                                                     uint64_t value)
{
        /* Width and sum of width and lsb both must be less than 64-bits */
        assert(width + lsb <= LONG_64);
        assert(width <= LONG_64);

        /* If the value does not fit in width unsigned bits */
        if (!Bitpack_fitsu(value, width)) {
                RAISE(Bitpack_Overflow);
        }

        /* Create mask for desired bits (using LONG_1 for 64-bit shifts) */
        uint64_t mask = ((LONG_1 << width) - LONG_1) << lsb;

        /* Clear desired bits */
        word &= ~mask;

        /* Set desired bits */
        word |= (value << lsb);

        return word;
}

/********** Bitpack_news ********
 *
 * Creates a new word which is identical to the original word, but the field 
 * of the given width and lsb will be replaced by a width-bit representation
 * of value.
 *
 * Parameters:
 *      uint64_t word:  the original codeword.
 *      unsigned width: the amount of bits for the updated field.
 *      unsigned lsb:   the least significant bit of the field.
 *      int64_t value: the given signed int that should fit in the width,
 * 
 * Return: 
 *      A 64-bit unsigned integer codeword.
 *
 * Expects:
 *      Width to be greater than or equal to 0, and less than or equal to 64.
 * 
 * Notes:
 *      Throws a C.R.E if called with width that is not 0 <= w <= 64.
 * 
 ************************/
uint64_t Bitpack_news(uint64_t word, unsigned width, unsigned lsb, 
                                                     int64_t value)
{
        /* Width and sum of width and lsb both must be less than 64-bits */
        assert(width + lsb <= LONG_64);
        assert(width <= LONG_64);

        /* If the value does not fit in width unsigned bits */
        if (!Bitpack_fitss(value, width)) {
                RAISE(Bitpack_Overflow);
        }

        /* Create mask for desired bits */
        uint64_t mask = ((LONG_1 << width) - LONG_1) << lsb;

        /* Clear desired bits */
        word &= ~mask;

        /* Discard sign extended bits to not mess OR operation and set bits */
        word |= (((uint64_t)value << (LONG_64 - width)) >> 
                                     (LONG_64 - width)) << lsb;

        return word;
}
