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

#include "BatchMandelCalculator.h"

BatchMandelCalculator::BatchMandelCalculator (unsigned matrixBaseSize, unsigned limit) :
    BaseMandelCalculator(matrixBaseSize, limit, "BatchMandelCalculator")
{
    // @TODO allocate & prefill memory
}

BatchMandelCalculator::~BatchMandelCalculator() {
    // @TODO cleanup the memory
}


int * BatchMandelCalculator::calculateMandelbrot () {
    // @TODO implement the calculator & return array of integers
    return NULL;
}
