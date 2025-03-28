#include "imagegenerator.h"

#include <cmath>
#include <filesystem>
#include <fstream>

struct RGB {
    uint8_t blue;
    uint8_t green;
    uint8_t red;
    uint8_t reserved;
};

// Палитра из 5 цветов
RGB palette[5] = {
    {0xFF, 0xFF, 0xFF, 0},  // Белый
    {0x6A, 0xA7, 0x47, 0},  // Зелёный
    {0x7A, 0x37, 0x53, 0},  // Фиолетовый
    {0x00, 0xA9, 0xF4, 0},  // Жёлтый
    {0x00, 0x00, 0x00, 0}   // Чёрный
};

void writeBMP(uint64_t width, uint64_t height, uint64_t **pixelData, uint64_t filename, const char *output_file) {
    uint64_t rowSize = ((width + 1) / 2 + 3) & ~3;
    uint64_t dataSize = rowSize * height;
    uint64_t fileSize = 54 + 5 * 4 + dataSize;  // Заголовок + палитра + данные

    char file_path[256] = "results/";
    if (filename == 0) {
        size_t len = std::strlen(output_file);
        std::strcat(file_path, output_file);
        if (len < 4 || std::strcmp(output_file + len - 4, ".bmp") != 0) {
            std::strcat(file_path, ".bmp");
        }
    } else {
        char filename_str[21];
        std::snprintf(filename_str, sizeof(filename_str), "%llu", filename);
        std::strcat(file_path, filename_str);
        std::strcat(file_path, ".bmp");
    }

    std::filesystem::path folder = "results";
    if (!std::filesystem::exists(folder)) {
        if (std::filesystem::create_directories(folder)) {
            std::cout << "Folder results created." << std::endl;
        }
    }

    // Открываем файл
    std::ofstream file(file_path, std::ios::binary);
    if (!file)
        throw std::invalid_argument("Error opening file.");

    // BMP заголовок
    uint8_t bmpHeader[54] = {
        'B', 'M',
        (uint8_t)(fileSize), (uint8_t)(fileSize >> 8), (uint8_t)(fileSize >> 16), (uint8_t)(fileSize >> 24),  // Размер файла
        0, 0, 0, 0,                                                                                           // Зарезервированные
        54 + 5 * 4, 0, 0, 0,                                                                                  // dataOffset
        40, 0, 0, 0,                                                                                          // headerSize
        (uint8_t)(width), (uint8_t)(width >> 8), (uint8_t)(width >> 16), (uint8_t)(width >> 24),              // Ширина
        (uint8_t)(height), (uint8_t)(height >> 8), (uint8_t)(height >> 16), (uint8_t)(height >> 24),          // Высота
        1, 0,                                                                                                 // planes
        4, 0,                                                                                                 // bitCount
        0, 0, 0, 0,                                                                                           // compression
        (uint8_t)(dataSize), (uint8_t)(dataSize >> 8), (uint8_t)(dataSize >> 16), (uint8_t)(dataSize >> 24),  // imageSize
        0, 0, 0, 0,                                                                                           // xPixelsPerMeter
        0, 0, 0, 0,                                                                                           // yPixelsPerMeter
        5, 0, 0, 0,                                                                                           // colorsUsed
        0, 0, 0, 0                                                                                            // colorsImportant
    };
    file.write((const char *)bmpHeader, 54);

    // Записываем палитру цветов
    for (const auto &color : palette)
        file.write((const char *)(&color), sizeof(RGB));

    // Записываем пиксели
    uint64_t width_div_two = std::ceil((float)width / 2);
    uint64_t padding_size = (4 - width_div_two % 4) % 4;

    for (uint16_t y = 0; y < height; ++y) {
        for (uint16_t x = 0; x < width; x += 2) {
            uint8_t byte = 0;
            byte |= (pixelData[(height - y - 1)][x]) << 4;
            if (x + 1 < width) {
                byte |= (pixelData[(height - y - 1)][x + 1]);
            }
            file.write((const char *)(&byte), 1);
        }
        file.write("0", padding_size);
    }

    file.close();
}
