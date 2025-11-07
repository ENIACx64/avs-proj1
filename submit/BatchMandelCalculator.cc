/**
 * @file BatchMandelCalculator.cc
 * @author Jindrich Vodak <xvodak06@stud.fit.vutbr.cz>
 * @brief Implementation of Mandelbrot calculator that uses SIMD paralelization over small batches
 * @date 2024-04-11
 */

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include <stdlib.h>
#include <stdexcept>

#include "BatchMandelCalculator.h"

BatchMandelCalculator::BatchMandelCalculator (unsigned matrixBaseSize, unsigned limit) :
	BaseMandelCalculator(matrixBaseSize, limit, "BatchMandelCalculator")
{
	// @TODO allocate & prefill memory
	data = (int *)(malloc(height * width * sizeof(int)));
}

BatchMandelCalculator::~BatchMandelCalculator() {
	// @TODO cleanup the memory
	free(data);
	data = NULL;
}

template <typename T>
static inline int mandelbrot(T real, T imag, int limit)
{
	T zReal = real;
	T zImag = imag;

	for (int i = 0; i < limit; ++i)
	{
		T r2 = zReal * zReal;
		T i2 = zImag * zImag;

		if (r2 + i2 > 4.0f)
			return i;

		zImag = 2.0f * zReal * zImag + imag;
		zReal = r2 - i2 + real;
	}
	return limit;
}

int *BatchMandelCalculator::calculateMandelbrot()
{
    int *pdata = data;
    block = 64;

    for (int ii = 0; ii < height; ii += block) {
        for (int jj = 0; jj < width; jj += block) {
            for (int i = ii; i < std::min(ii + block, height); i++) {
                #pragma omp simd
                for (int j = jj; j < std::min(jj + block, width); j++) {
                    float x = x_start + j * dx; // current real value
                    float y = y_start + i * dy; // current imaginary value

                    int value = mandelbrot(x, y, limit);

                    pdata[i * width + j] = value;
                }
            }
        }
    }
    return data;
}

