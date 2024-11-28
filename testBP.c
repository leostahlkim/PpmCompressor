// File for testing our bitpacking module


#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <assert.h>
#include <bitpack.h>


int main() 
{
        assert(Bitpack_fitsu(12, 4));

        /* Test fit unsigned */
        assert(Bitpack_fitsu(5, 3));
        assert(!Bitpack_fitsu(5, 2));

        /* Test fit signed */
        fprintf(stderr, "signed 3 fit into 2 bits?: %d\n", Bitpack_fitss(3, 2));
        fprintf(stderr, "TEST2: %d\n", Bitpack_fitss(-1, 2));


        /* Test unsigned get */
        
        //General Case from Spec
        assert(Bitpack_getu(1012, 6, 2) == 61);

        // When width is max width of 64 and the word is an int max
        uint64_t max_val = ~0;
        assert(Bitpack_getu(max_val, 64, 0) == max_val);

        assert(Bitpack_getu(0, 64, 0) == 0);

        // Get goes past the most significant one of the original
        assert(Bitpack_getu(768, 3, 8) == 3);

        // Completely goes past original 
        assert(Bitpack_getu(768, 54, 10) == 0);

        //Attempting to get bits out of range
        //Bitpack_getu(768, 55, 10);

        /* Test signed get */

        //General Cases
        assert(Bitpack_gets(21, 2, 4) == 1);
        assert(Bitpack_gets(21, 3, 2) == -3);
        assert(Bitpack_gets(1012, 6, 2) == -3);
        assert(Bitpack_gets(1012, 4, 0) == 4);


        /* Test bitpack new unsigned */

        //General Cases
        assert(Bitpack_newu(768, 2, 8, 0) == 0);

        assert(Bitpack_newu(63, 6, 0, 1) == 1);

        //New value doesn't fit in width bits
        //Bitpack_newu(768, 2, 7, 4);

        /* test bitpack new signed */
        printf("number is %d\n", Bitpack_news(768, 2, 8, -1) == 768);
        assert(Bitpack_news(768, 2, 8, -1) == 768);


       
        #define word 555
        #define  w 5
        #define lsb 6
        #define val 15
        #define w2 9
        #define lsb2 10

        /* Test Identities */
        assert(Bitpack_getu(Bitpack_newu(word, w, lsb, val), w, lsb) == val);


        assert(Bitpack_getu(Bitpack_newu(word, w, lsb, val), w2, lsb2) == Bitpack_getu(word, w2, lsb2));




        // /* tesst for unsigned fit */
        // assert(Bitpack_fitsu(0, 3));
        // assert(Bitpack_fitsu(5, 3));
        // assert(Bitpack_fitsu(9, 64));
        // assert(Bitpack_fitsu(1, 1));
        // assert(Bitpack_fitsu(0, 1));
        // assert(Bitpack_fitsu(0, 3));
        // assert(Bitpack_fitsu(7, 3));
        // assert(!Bitpack_fitsu(8, 3));
        // assert(!(Bitpack_fitsu(3, 0)));
        // assert(!(Bitpack_fitsu(8, 3)));
        // assert(!(Bitpack_fitsu(4, 2)));
        // assert(!(Bitpack_fitsu(998, 5)));
        // assert(!(Bitpack_fitsu(8, 65)));

        // /* test for signed fit */
        // assert(Bitpack_fitss(0, 3));
        // assert(Bitpack_fitss(-3, 3));
        // assert(Bitpack_fitss(2, 3));
        // assert(Bitpack_fitss(-4, 3));
        // assert(Bitpack_fitss(3, 3));
        // assert(Bitpack_fitss(-9, 64));
        // assert(Bitpack_fitss(9, 64));
        // assert(Bitpack_fitss(-1, 1));
        // assert(Bitpack_fitss(0, 1));
        // assert(!Bitpack_fitss(1, 1));
        // assert(!Bitpack_fitss(-5, 3));
        // assert(!Bitpack_fitss(4, 3));
        // assert(!Bitpack_fitss(0, 0));
        // assert(Bitpack_fitss(0, 4));

        // // /* test for left shift */
        // // assert(lshift(9, 64) == 0);
        // // assert(lshift(0, 4) == 0);
        // // assert(lshift(23, 69) == 0);
        // // assert(lshift(4, 6) == 256);
        // // assert((int64_t)lshift(-1, 4) == -16);
        // // assert((int64_t)lshift(-1, 69) == 0);

        // // /* test for unsigned rshift */
        // // assert(u_rshift(64, 2) == 16);
        // // assert(u_rshift(23, 64) == 0);
        // // assert(u_rshift(1298, 69) == 0);
        // // assert(u_rshift(534, 1) == 534/2);
        // // assert(u_rshift(2367, 5) == 2367/32);

        // // /* test for signed rshift */
        // // assert(s_rshift(64, 2) == 16);
        // // assert(s_rshift(-23, 64) == -1);
        // // assert(s_rshift(-1298, 69) == -1);
        // // assert(s_rshift(1298, 69) == 0);
        // // assert(s_rshift(-534, 1) == -534/2);
        // // assert(s_rshift(2368, 5) == 2368/(32));
        // // assert(s_rshift(-2368, 5) == -2368/(32));
        // // assert(s_rshift(-24, 2) == -6);
        // // assert(s_rshift(9, 64) == 0);

        // // // /* test for mask */
        // // assert(new_extraction_mask(2, 1) == 6);
        // // assert(new_extraction_mask(2, 0) == 3);
        // // assert(new_extraction_mask(2, 5) == 96);
        // // assert(new_extraction_mask(0, 5) == 96);


        // /* tests for getU */
        // // Bitpack_getu(0x3f4, 65, 2);
        // // Bitpack_getu(0x3f4, 50, 26);
        // assert(Bitpack_getu(0x3f4, 0, 2) == 0);
        // assert(Bitpack_getu(0x3f4, 6, 2) == 61);
        // // printf("hello: %ld\n", (int64_t)(~Bitpack_getu(0x3f4, 6, 2)));
        // // assert((int64_t)(~Bitpack_getu(0x3f4, 6, 2)) == -3);
        // assert(Bitpack_getu(0, 32, 2) == 0);
        // assert(Bitpack_getu(~0, 1, 63) == 1);
        // assert(Bitpack_getu(~0, 0, 64) == 0);
        // assert(Bitpack_getu(~0, 1, 0) == 1);
        // assert(Bitpack_getu(~0, 2, 0) == 3);
        // assert(Bitpack_getu(~0, 3, 34) == 7);
        // assert(Bitpack_getu(~0, 3, 34) == 7);
        // assert(Bitpack_getu(0x0abcd00800000000, 16, 44) ==            
        //                     0x000000000000abcd);
        // // assert(Bitpack_getu(0x3f4, 6, 2) == 61);
        
        // /* test for get signed */
        // assert(Bitpack_gets(0x3f4, 6, 2) == -3);
        // assert(Bitpack_gets(~0, 12, 32) == -1);
        // assert(Bitpack_gets(~0, 12, 52) == -1);
        // assert(Bitpack_gets(0xfffeffffffffffff, 8, 48) == -2);
        // // assert(Bitpack_gets(-12, 2, 1) == );
        // // assert(Bitpack_gets(0x3f4, 6, 2) == -3);
        // // assert(Bitpack_gets(0x3f4, 6, 2) == -3);

        /* test Bitpack_newu */
        assert(Bitpack_newu(0x000000000000000, 4, 8, 7) == 0x000000000000700);
        assert(Bitpack_newu(0x0a0000008000000, 4, 12, 7) ==            
                            0x0a0000008007000);
        assert(Bitpack_newu(0x0a01200008009000, 4, 56, 12) ==            
                            0x0c01200008009000);
        assert(Bitpack_newu(0x5a01200008009000, 4, 60, 5) ==            
                            0x5a01200008009000);
        assert(Bitpack_newu(0x00a0120000800900, 4, 48, 12) ==            
                            0x00ac120000800900);
        assert(Bitpack_newu(0x00a00000080090f0, 4, 0, 11) ==            
                            0x00a00000080090fb);
        assert(Bitpack_newu(0x00020e00060000f0, 8, 0, 17) ==            
                            0x00020e0006000011);
        assert(Bitpack_getu(Bitpack_newu(69, 5, 2, 6), 5, 2) == 6);
        assert(Bitpack_getu(Bitpack_newu(69, 4, 10, 8), 10, 15) == Bitpack_getu(69, 10, 15));

        /* tests news */
        assert(Bitpack_news(0x0a0000008000000, 4, 12, -1) ==            
                            0x0a000000800f000);
        assert(Bitpack_news(0x0a01200008009000, 4, 56, -1) ==            
                            0x0f01200008009000);
        assert(Bitpack_news(0x4500000008009000, 4, 60, -1) ==            
                            0xf500000008009000);

        // assert(Bitpack_news(0x00a0120000800900, 4, 48, 12) ==            
        //                     0x00ac120000800900);
        // assert(Bitpack_news(0x00a00000080090f0, 4, 0, 11) ==            
        //                     0x00a00000080090fb);
        // assert(Bitpack_news(0x00020e00060000f0, 8, 0, 17) ==            
                        //     0x00020e0006000011);

        
        printf("HELOOOOOO\n");

        

        return 0;
}


// uint64_t u_rshift(uint64_t n, unsigned shift)
// {
//         if (shift >= 64) {
//                 return (uint64_t)0;
//         }
        
//         return n >> shift;
// }


// uint64_t lshift(uint64_t n, unsigned shift)
// {
//         if (shift >= 64) {
//                 return (uint64_t)0;
//         }
        
//         return n << shift;
// }

// int64_t s_rshift(int64_t n, unsigned shift)
// {
//         if (shift >= 64) {
//                 if (n < 0) {
//                         return (int64_t)-1;
//                 } else {
//                         return (int64_t)0;
//                 }
//         }
//         return n >> shift;
// }

// uint64_t new_extraction_mask(unsigned width, unsigned lsb)
// {
//         uint64_t mask = 0;
//         static uint64_t WORDLEN = 64;

//         mask = ~mask;
//         mask = u_rshift(mask, WORDLEN - width);
//         mask = lshift(mask, lsb);
        
//         return mask;
// }