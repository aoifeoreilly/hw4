/**************************************************************
 *
 *                     bitpack.c
 *
 *      Assignment: arith
 *      Authors:    Griffin Faecher (gfaech01) and Aoife O'Reilly (aoreil02)
 *      Date:       2/21/2025
 *
 *      This file
 *
 *************************************************************/

#include <bitpack.h>
#include <stdio.h>
#include <assert.h>
#include <except.h>

Except_T Bitpack_Overflow = { "Overflow packing bits" };


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
        uint64_t max = (0x0000000000000001u << width) - 1;
        printf("n: %lu | max: %u\n", n, (0x0000000000000001u << width) - 1);
        return n <= max;
}

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
        int64_t max = (0x0000000000000001u << (width - 1)) - 1;
        int64_t min = ~max;
        return n <= max && n >= min;

}

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
        /* Spec says to do this MIGHT NEED SOMETHING ELSE IDK */
        assert(width + lsb <= 64);
        assert(width <= 64);
        if (!Bitpack_fitsu(value, width)) {
                RAISE(Bitpack_Overflow);
        }
        /* Create mask for desired bits */
        uint64_t mask = ((1 << width) - 1) << lsb;
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
        uint64_t mask = ((1 << width) - 1) << lsb;
        /* Clear desired bits */
        word &= ~mask;
        /* Get rid of sign extended bits to not mess with or operation and set 
        bits */
        word |= (((value << (64 - width)) >> (64 - width)) << lsb);
        return word;

}
