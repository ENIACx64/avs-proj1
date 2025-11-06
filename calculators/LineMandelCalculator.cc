/**
 * @file LineMandelCalculator.cc
 * @author Jindrich Vodak <xvodak06@stud.fit.vutbr.cz>
 * @brief Implementation of Mandelbrot calculator that uses SIMD paralelization over lines
 * @date 06/11/2025
 */
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include <stdlib.h>
#include <cstring>
#include <mm_malloc.h>


#include "LineMandelCalculator.h"


LineMandelCalculator::LineMandelCalculator (unsigned matrixBaseSize, unsigned limit) :
    BaseMandelCalculator(matrixBaseSize, limit, "LineMandelCalculator")
{
    int set_size = height * width * sizeof(int);
    data = (int *) _mm_malloc(set_size, 64);
    memset(data, 0, set_size);
}

LineMandelCalculator::~LineMandelCalculator() {
    _mm_free(data);
    data = NULL;
}


int * LineMandelCalculator::calculateMandelbrot () {
    for (int i = 0; i < height / 2; i++)
    {
        float y = y_start + i * dy;
        int row = i * width;

        #pragma omp simd
        for (int j = row; j < (row + width); j++)
        {
            int k = j - row;
            float x = x_start + k * dx;
            float z_real = x;
            float z_imaginary = y;

            for (int l = 1; l < limit; ++l)
            {
                float real_sq = z_real * z_real;
                float imaginary_sq = z_imaginary * z_imaginary;

                // threshold
                int threshold = (real_sq + imaginary_sq > 4.0f);

                data[j] += !threshold;

                if (threshold)
                {
                    break;
                }

                float temp = z_real;
                z_imaginary = 2.0f * z_real * z_imaginary + y;
                z_real = real_sq - imaginary_sq + x;
            }
        }
    }

    for (int i = 0; i < height / 2; i++)
    {
        memcpy(data + width * (height - i - 1), data + width * i, width * sizeof(int));
    }

    return data;
}
