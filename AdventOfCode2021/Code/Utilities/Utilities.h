#pragma once

#include "Types.h"
#include "FullParsers.h"
#include "PartialParsers.h"

#if true

#include <chrono>

#define MEASURE(what) auto what##1 = std::chrono::high_resolution_clock::now();
#define MEASURE_END(what) auto what##2 = std::chrono::high_resolution_clock::now(); \
	printf(#what ": %f\n", std::chrono::duration<double, std::milli>(what##2 - what##1).count());

#else

#define MEASURE(what)
#define MEASURE_END(what)

#endif