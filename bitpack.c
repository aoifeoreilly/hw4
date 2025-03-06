/**************************************************************
 *
 *                     bitpack.c
 *
 *      Assignment: arith
 *      Authors:    Griffin Faecher (gfaech01) and Aoife O'Reilly (aoreil02)
 *      Date:       3/5/2025
 *
 *      This file
 *
 *************************************************************/

#include <bitpack.h>
#include <stdio.h>
#include <assert.h>
#include <except.h>

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
 *     
 * 
 * Notes:
 *     
 ************************/
bool Bitpack_fitsu(uint64_t n, unsigned width)
{
        /* Spec says to do this MIGHT NEED SOMETHING ELSE IDK */
        assert(width <= 64);
        if (width == 0) {
                return false;
        }
        if (width == 64) {
                return true;
        }
        uint64_t max = (1ULL << width) - 1;
        return n <= max;
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
 *     
 * 
 * Notes:
 *     
 ************************/
bool Bitpack_fitss(int64_t n, unsigned width)
{
        /* Spec says to do this MIGHT NEED SOMETHING ELSE IDK */
        assert(width <= 64);
        /* Width cannot be 0 bits. Also, if width = 64, all data should fit */
        if (width == 0) {
                return false;
        }
        if (width == 64) {
                return true;
        }
        /* Find the min and max values given width, and check to see if n fits*/
        int64_t max = (1ULL << (width - 1)) - 1;
        int64_t min = ~max;
        return n <= max && n >= min;

}

/********** Bitpack_getu ********
 *
 * Extracts a field from a word given the width of the field and the location 
 * of the field's least significant bit.
 *
 * Parameters:
 *      uint64_t word:
 *      unsigned width:
 *      unsigned lsb:
 * 
 * Return: 
 *
 * Expects:
 *     
 * 
 * Notes:
 *     This function will C.R.E if called with width that is not 0 <= w <= 64.
 * 
 ************************/
uint64_t Bitpack_getu(uint64_t word, unsigned width, unsigned lsb)
{
        /* Spec says to do this MIGHT NEED SOMETHING ELSE IDK */
        assert(width + lsb <= 64);
        assert(width <= 64);
        if (width == 0) {
                return 0;
        }
        /* Position mask in correct space in the word */
        uint64_t mask = ((1 << width) - 1) << lsb;
        /* Return the bits specified by user, and shift back to lsb of the 64
        bit word */
        return (word & mask) >> lsb;

}

int64_t Bitpack_gets(uint64_t word, unsigned width, unsigned lsb)
{
        /* Spec says to do this MIGHT NEED SOMETHING ELSE IDK */
        assert(width + lsb <= 64);
        assert(width <= 64);
        if (width == 0) {
                return 0;
        }
        /* Position mask in correct space in the word */
        uint64_t mask = ((1 << width) - 1) << lsb;
        /* Find bits specified by user and shift them to lsb of the 64 bit 
        word */
        int64_t value = (word & mask) >> lsb;
        /* If the number is signed, extend the sign */
        if (value & (1 << (width - 1))) {
                value = value | ~(mask >> lsb);
        }
        return value;

}

uint64_t Bitpack_newu(uint64_t word, unsigned width, unsigned lsb, uint64_t value)
{
    /* Check preconditions */
    assert(width + lsb <= 64);
    assert(width <= 64);

    if (!Bitpack_fitsu(value, width)) {
        RAISE(Bitpack_Overflow);
    }

    /* Create mask for desired bits (using 1ULL for 64-bit shifts) */
    uint64_t mask = ((1ULL << width) - 1) << lsb;

    /* Clear desired bits */
    word &= ~mask;

    /* Set desired bits */
    word |= (value << lsb);

    return word;
}

uint64_t Bitpack_news(uint64_t word, unsigned width, unsigned lsb,  int64_t value)
{
        /* Spec says to do this MIGHT NEED SOMETHING ELSE IDK */
        assert(width + lsb <= 64);
        assert(width <= 64);
        if (!Bitpack_fitss(value, width)) {
                RAISE(Bitpack_Overflow);
        }
        /* Create mask for desired bits */
        uint64_t mask = ((1ULL << width) - 1ULL) << lsb;
        /* Clear desired bits */
        word &= ~mask;
        /* Get rid of sign extended bits to not mess with or operation and set 
        bits */
        word |= (((uint64_t)value << (64 - width)) >> (64 - width)) << lsb;
        return word;

}
