#pragma once
#include <cstdint>
#include <cstring>
#include <iostream>

bool CheckDigit(const char* str);

struct Parser {
    const char* input_file = "input.tsv";
    const char* output_file = "output.bmp";
    uint64_t frequency = 0;
    uint64_t max_iteration = UINT64_MAX;

    Parser(int argc, char** argv);
    void Print();
};
