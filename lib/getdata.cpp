#include "getdata.h"

#include <cstring>
#include <fstream>

Data GetDataFromTSV(Parser parser) {
    Data result;

    // Добавление расширения, если его нет
    if (std::strlen(parser.input_file) < 4 || std::strcmp(parser.input_file + std::strlen(parser.input_file) - 4, ".tsv") != 0) {
        char* temp_file_name = new char[std::strlen(parser.input_file) + 5];
        std::strcpy(temp_file_name, parser.input_file);
        std::strcat(temp_file_name, ".tsv");
        parser.input_file = temp_file_name;
    }

    char file_path[256] = "data/";
    std::strcat(file_path, parser.input_file);
    std::ifstream file_input(file_path);
    if (!file_input.is_open()) {
        throw std::invalid_argument("Error opening file.");
    }

    char line[256];
    while (file_input.getline(line, 256)) {
        Pixel pixel;
        char* token = std::strtok(line, "\t");
        int number_of_argument = 0;

        // Парсинг строки
        while (token != nullptr) {
            if (number_of_argument == 0) {
                pixel.x = static_cast<uint16_t>(std::atoi(token));
                number_of_argument++;
            } else if (number_of_argument == 1) {
                pixel.y = static_cast<uint16_t>(std::atoi(token));
                number_of_argument++;
            } else if (number_of_argument == 2) {
                pixel.count_grains = static_cast<uint64_t>(std::atoi(token));
            }
            token = std::strtok(nullptr, "\t");
        }

        result.AddPixel(pixel);
    }

    if (parser.input_file != "input.tsv" && parser.input_file != "output.tsv") {
        delete[] parser.input_file;
    }

    return result;
}
