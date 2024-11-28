/**************************************************************
 *
 *                     ppmdiff.c
 *
 *     Authors:  Eric Zhao, Leo Kim
 *     Date:     October 20, 2022
 *
 *     Summary:
 *     The purpose of this program is to assess the difference
 *     between two ppm difference and assign a score
 *     
 *
 **************************************************************/
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <assert.h>
#include <a2methods.h>
#include <a2plain.h>
#include <a2blocked.h>
#include "pnm.h"

/* RENAME CLOSURE */
struct closure {
        unsigned min_width;
        unsigned min_height;
        double diffSum;
        Pnm_ppm ppm_image1;
        Pnm_ppm ppm_image2;
        A2Methods_T methods;
};

bool validate_dimensions(int dimension1, int dimension2);
void compare_pixels(int col, int row, A2Methods_UArray2 image1,
                    void *val, void *cl);
double rms_difference(Pnm_ppm pixmap1, Pnm_ppm pixmap2, A2Methods_T methods);

double RGB_sqr_diff(Pnm_rgb image1_pix, Pnm_rgb image2_pix, 
                    double *denominator_image1, 
                    double *denominator_image2);


int main(int argc, char *argv[])
{
        assert(argc == 3);

        /* set method to default method */
        A2Methods_T methods = uarray2_methods_plain; 
        assert(methods);

        FILE *image1, *image2;

        double percentDiff;

        if (strcmp(argv[1], "-") == 0) {
                image1 = stdin;
        } else {
                image1 = fopen(argv[1], "rw");
                if (image1 == NULL) {
                        fprintf(stderr, 
                                "File cannot be opened for reading\n");
                        exit(EXIT_FAILURE);
                }
        }

        if (strcmp(argv[2], "-") == 0) {
                image2 = stdin;
        } else {
                image2 = fopen(argv[2], "rw");
                if (image2 == NULL) {
                        fprintf(stderr, 
                                "File cannot be opened for reading\n");
                        exit(EXIT_FAILURE);
                }
        }

        Pnm_ppm pixmap1 = Pnm_ppmread(image1, methods);
        fclose(image1);
        Pnm_ppm pixmap2 = Pnm_ppmread(image2, methods);
        fclose(image2);

        if (validate_dimensions(pixmap1->width, pixmap2->width) && 
            validate_dimensions(pixmap2->height, pixmap2->height)) {
                percentDiff = rms_difference(pixmap1, pixmap2, methods);
                
        } else {
                percentDiff = 1.0;
        }

        printf("%f\n", percentDiff);

        Pnm_ppmfree(&pixmap1);
        Pnm_ppmfree(&pixmap2);

        return 0;
}

bool validate_dimensions(int dimension1, int dimension2) 
{
        if (abs(dimension1 - dimension2) > 1) {
                fprintf(stderr, 
                "DIMENSIONS OF IMAGES VARY BY MORE THAN 1 PIXEL\n");
                return false;
        }
        return true;
}

double rms_difference(Pnm_ppm pixmap1, Pnm_ppm pixmap2, A2Methods_T methods)
{
        unsigned min_width = pixmap1->width;
        unsigned min_height = pixmap1->height;
        
        if (pixmap2->width < pixmap1->width) {
                min_width = pixmap2->width;
        }
        if (pixmap2->height < pixmap1->height) {
                min_height = pixmap2->height;
        }

        struct closure *cl = malloc(sizeof(*cl));
        assert(cl);

        /* update struct fields */
        cl->min_width = min_width;
        cl->min_height = min_height;
        cl->diffSum = 0;
        cl->ppm_image1 = pixmap1;
        cl->ppm_image2 = pixmap2;
        cl->methods = methods;
        
        methods->map_default(pixmap1->pixels, compare_pixels, cl);

        // printf("diffSum: %f\n", cl->diffSum);

        double rms_diff = sqrt(cl->diffSum / (3 * min_width * min_height));

        free(cl);
        return rms_diff;
}

void compare_pixels(int col, int row, A2Methods_UArray2 image1, 
                    void *val, void *cl)
{
        int min_width = ((struct closure *)cl)->min_width;
        int min_height = ((struct closure *)cl)->min_height;
 
        if ((col < min_width) && (row < min_height)) {
                /* image2 uarray2 */
                A2Methods_UArray2 image2 = 
                        ((struct closure *)cl)->ppm_image2->pixels;

                /* image1 pixel at col, row */
                Pnm_rgb pixel_image1 = (Pnm_rgb) val;

                /* image2 pixel at col, row */
                Pnm_rgb pixel_image2 = (Pnm_rgb) 
                (((struct closure *)cl)->methods->at(image2, col, row));

                /* denominator of image1 */
                double denominator_image1 =
                        (((struct closure *)cl)->ppm_image1->denominator);

                /* denominator of image2 */
                double denominator_image2 = 
                        ((struct closure *)cl)->ppm_image2->denominator;

                double *diffSum = &(((struct closure *)cl)->diffSum);

                (*diffSum) += RGB_sqr_diff(pixel_image1, pixel_image2, 
                                           &denominator_image1, 
                                           &denominator_image2);
        }
        (void) image1;
        (void) val;
}


double RGB_sqr_diff(Pnm_rgb image1_pix, Pnm_rgb image2_pix, 
                    double *denominator_image1, 
                    double *denominator_image2)
{
        double R_diff = ((double)image1_pix->red / (*denominator_image1) - 
                         (double)image2_pix->red / (*denominator_image2));
        double R_sqr_diff = R_diff * R_diff;


        double G_diff = ((double)image1_pix->green / (*denominator_image1) - 
                         (double)image2_pix->green / (*denominator_image2));
        double G_sqr_diff = G_diff * G_diff;


        double B_diff = ((double)image1_pix->blue / (*denominator_image1) - 
                         (double)image2_pix->blue / (*denominator_image2));
        double B_sqr_diff = B_diff * B_diff;
                
                
        return (R_sqr_diff + G_sqr_diff + B_sqr_diff);
}