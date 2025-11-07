/**
 * @file BatchMandelCalculator.cc
 * @author Jindrich Vodak <xvodak06@stud.fit.vutbr.cz>
 * @brief Implementation of Mandelbrot calculator that uses SIMD paralelization over small batches
 * @date 06/11/2025
 */

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include <stdlib.h>
#include <stdexcept>
#include <cstring>
#include <mm_malloc.h>

#include "BatchMandelCalculator.h"

BatchMandelCalculator::BatchMandelCalculator (unsigned matrixBaseSize, unsigned limit) :
    BaseMandelCalculator(matrixBaseSize, limit, "BatchMandelCalculator")
{
    int set_size = height * width * sizeof(int);
    data = (int *) _mm_malloc(set_size, 64);
    memset(data, 0, set_size);
}

BatchMandelCalculator::~BatchMandelCalculator() {
    _mm_free(data);
    data = NULL;
}


int * BatchMandelCalculator::calculateMandelbrot () {
    constexpr int TILE_SIZE = 32;

    for (int i = 0; i < height / 2; i += TILE_SIZE)
    {
        for (int j = i; j < std::min(i + TILE_SIZE, height / 2); j++)
        {
            float y = y_start + j * dy;
            int row = j * width;

            #pragma omp simd
            for (int k = row; k < (row + width); k++)
            {
                int l = k - row;
                float x = x_start + l * dx;
                float z_real = x;
                float z_imaginary = y;

                for (int m = 1; m < limit; ++m)
                {
                    float real_sq = z_real * z_real;
                    float imaginary_sq = z_imaginary * z_imaginary;

                    // threshold
                    int threshold = (real_sq + imaginary_sq > 4.0f);

                    data[k] += !threshold;

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
    }

    for (int i = 0; i < height / 2; i++)
    {
        memcpy(data + width * (height - i - 1), data + width * i, width * sizeof(int));
    }

    return data;
}
