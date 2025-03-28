#pragma once

#include <cstdint>
#include <cstring>

#include "getdata.h"
void writeBMP(uint64_t width, uint64_t height, uint64_t** pixelData, uint64_t filename, const char* output_file);
