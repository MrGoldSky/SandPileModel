#pragma once

#include "getdata.h"
#include "parser.h"

struct SandPile {
    char *input_file;
    char *output_file;
    uint64_t frequency;
    uint64_t max_iteration;

    uint64_t iterations = 0;

    uint64_t height;
    uint64_t width;
    uint64_t shift_x = 0;
    uint64_t shift_y = 0;

    bool is_stable;

    uint64_t **matrix = nullptr;
    uint64_t **matrix_clone = nullptr;

    SandPile(Parser parser, Data pixels);
    void PrintMatrix();
    bool CheckStable();

    void Collapse();
    void CollapseLeft(uint64_t x, uint64_t y);
    void CollapseRight(uint64_t x, uint64_t y);
    void CollapseUp(uint64_t x, uint64_t y);
    void CollapseDown(uint64_t x, uint64_t y);

    void FromClone();

    void allocate(const char *direction);

    void SaveToBMP(uint64_t filename, const char *output_file);
    void DeleteWhiteBorder(uint64_t &min_x, uint64_t &max_x, uint64_t &min_y, uint64_t &max_y);
};