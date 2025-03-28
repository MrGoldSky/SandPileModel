#pragma once

#include "parser.h"

struct Pixel {
    int16_t x = 0;
    int16_t y = 0;
    uint64_t count_grains = 0;
};

struct Data {
    Pixel* pixels = new Pixel[1];

    int16_t minimal_x = INT16_MAX;
    int16_t maximal_x = INT16_MIN;
    int16_t minimal_y = INT16_MAX;
    int16_t maximal_y = INT16_MIN;

    size_t size = 0;
    size_t capacity = 1;

    void CheckLimits(Pixel pixel) {
        minimal_x = std::min(minimal_x, pixel.x);
        maximal_x = std::max(maximal_x, pixel.x);
        minimal_y = std::min(minimal_y, pixel.y);
        maximal_y = std::max(maximal_y, pixel.y);
    }

    void AddPixel(const Pixel& pixel) {
        if (size >= capacity) {
            size_t new_capacity = capacity * 2;
            Pixel* new_pixels = new Pixel[new_capacity];
            std::memcpy(new_pixels, pixels, size * sizeof(Pixel));

            delete[] pixels;
            pixels = new_pixels;
            capacity = new_capacity;
        }
        pixels[size++] = pixel;
        CheckLimits(pixel);
    }
};

Data GetDataFromTSV(Parser parser);