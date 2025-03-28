#include "sandpile.h"

#include "imagegenerator.h"
#include "parser.h"

// Конструктор песчанной кучи
SandPile::SandPile(Parser parser, Data data_pixels) {
    input_file = new char[std::strlen(parser.input_file) + 1];
    std::strcpy(input_file, parser.input_file);

    output_file = new char[std::strlen(parser.output_file) + 1];
    std::strcpy(output_file, parser.output_file);

    frequency = parser.frequency;
    max_iteration = parser.max_iteration;

    uint64_t shift_x = 0;
    uint64_t shift_y = 0;

    height = data_pixels.maximal_y - data_pixels.minimal_y + 1;
    width = data_pixels.maximal_x - data_pixels.minimal_x + 1;
    matrix = {new uint64_t *[height]};
    matrix_clone = {new uint64_t *[height]};

    for (size_t i = 0; i < height; ++i) {
        matrix[i] = new uint64_t[width]{0};
        matrix_clone[i] = new uint64_t[width]{0};
    }

    for (uint64_t pixel = 0; pixel < data_pixels.size; ++pixel) {
        uint64_t current_y = data_pixels.pixels[pixel].y - data_pixels.minimal_y;
        uint64_t current_x = data_pixels.pixels[pixel].x - data_pixels.minimal_x;
        matrix[height - current_y - 1][current_x] = data_pixels.pixels[pixel].count_grains;
        matrix_clone[height - current_y - 1][current_x] = data_pixels.pixels[pixel].count_grains;
    }
    delete[] data_pixels.pixels;
    is_stable = CheckStable();
}

void SandPile::Collapse() {
    uint64_t base_width = width;
    uint64_t base_height = height;
    iterations++;

    shift_x = 0;
    shift_y = 0;

    for (uint64_t y = 0; y < base_height; y++) {
        for (uint64_t x = 0; x < base_width; x++) {
            if (matrix[y + shift_y][x + shift_x] >= 4) {
                matrix[y + shift_y][x + shift_x] -= 4;
                matrix_clone[y + shift_y][x + shift_x] -= 4;

                CollapseLeft(x, y);
                CollapseRight(x, y);
                CollapseUp(x, y);
                CollapseDown(x, y);

                // PrintMatrix();
            }
        }
    }
    FromClone();
    is_stable = CheckStable();
}

void SandPile::FromClone() {
    for (uint64_t y = 0; y < height; y++) {
        for (uint64_t x = 0; x < width; x++) {
            matrix[y][x] = matrix_clone[y][x];
        }
    }
}

void SandPile::allocate(const char *direction) {
    float upload_matrix = 1.3;
    uint64_t new_width = static_cast<uint64_t>(width * upload_matrix);
    uint64_t new_height = static_cast<uint64_t>(height * upload_matrix);

    if (strcmp(direction, "right") == 0 || strcmp(direction, "left") == 0)
        new_width = std::max(new_width, width + 1);
    else if (strcmp(direction, "up") == 0 || strcmp(direction, "down") == 0)
        new_height = std::max(new_height, height + 1);

    uint64_t **matrix_allocated = new uint64_t *[new_height];
    uint64_t **matrix_clone_allocated = new uint64_t *[new_height];
    for (uint64_t i = 0; i < new_height; i++) {
        matrix_allocated[i] = new uint64_t[new_width]{0};
        matrix_clone_allocated[i] = new uint64_t[new_width]{0};
    }

    uint64_t x_offset = 0;
    uint64_t y_offset = 0;

    if (strcmp(direction, "left") == 0) {
        x_offset = new_width - width;
        shift_x = x_offset;
    } else if (strcmp(direction, "up") == 0) {
        y_offset = new_height - height;
        shift_y = y_offset;
    }

    for (uint64_t y = 0; y < height; y++) {
        for (uint64_t x = 0; x < width; x++) {
            matrix_allocated[y + y_offset][x + x_offset] = matrix[y][x];
            matrix_clone_allocated[y + y_offset][x + x_offset] = matrix_clone[y][x];
        }
        delete[] matrix[y];
        delete[] matrix_clone[y];
    }

    delete[] matrix;
    delete[] matrix_clone;

    matrix = matrix_allocated;
    matrix_clone = matrix_clone_allocated;
    width = new_width;
    height = new_height;
}

void SandPile::CollapseRight(uint64_t x, uint64_t y) {
    if (x + shift_x == width - 1)
        allocate("right");
    matrix_clone[y + shift_y][x + shift_x + 1]++;
}

void SandPile::CollapseLeft(uint64_t x, uint64_t y) {
    if (x == 0)
        allocate("left");
    matrix_clone[y + shift_y][x + shift_x - 1]++;
}

void SandPile::CollapseUp(uint64_t x, uint64_t y) {
    if (y == 0)
        allocate("up");
    matrix_clone[y + shift_y - 1][x + shift_x]++;
}

void SandPile::CollapseDown(uint64_t x, uint64_t y) {
    if (y + shift_y == height - 1)
        allocate("down");
    matrix_clone[y + shift_y + 1][x + shift_x]++;
}

bool SandPile::CheckStable() {
    for (uint64_t y = 0; y < height; y++) {
        for (uint64_t x = 0; x < width; x++) {
            if (matrix[y][x] >= 4)
                return false;
        }
    }
    return true;
}

void SandPile::PrintMatrix() {
    std::cout << "\nMatrix: " << '\n';
    for (uint64_t i = 0; i < width * 2; i++)
        std::cout << '=';
    std::cout << '\n';
    for (uint64_t y = 0; y < height; y++) {
        for (uint64_t x = 0; x < width; x++) {
            std::cout << matrix[y][x] << ' ';
        }
        std::cout << '\n';
    }

    for (uint64_t i = 0; i < width * 2; i++)
        std::cout << '=';
    std::cout << '\n';
    std::cout << "Stable: " << is_stable << '\n';
}

void SandPile::DeleteWhiteBorder(uint64_t &min_x, uint64_t &max_x, uint64_t &min_y, uint64_t &max_y) {
    min_x = width;
    max_x = 0;
    min_y = height;
    max_y = 0;
    bool no_white_pixels = false;

    for (uint64_t y = 0; y < height; ++y) {
        for (uint64_t x = 0; x < width; ++x) {
            if (matrix[y][x] != 0) {
                no_white_pixels = true;
                if (x < min_x)
                    min_x = x;
                if (x > max_x)
                    max_x = x;
                if (y < min_y)
                    min_y = y;
                if (y > max_y)
                    max_y = y;
            }
        }
    }

    if (!no_white_pixels) {
        min_x = 0;
        max_x = width - 1;
        min_y = 0;
        max_y = height - 1;
    }
}

void SandPile::SaveToBMP(uint64_t filename, const char *output_file) {
    uint64_t min_x;
    uint64_t max_x;
    uint64_t min_y;
    uint64_t max_y;
    DeleteWhiteBorder(min_x, max_x, min_y, max_y);

    uint64_t cropped_width = max_x - min_x + 1;
    uint64_t cropped_height = max_y - min_y + 1;

    uint64_t **cropped_matrix = new uint64_t *[cropped_height];
    for (uint64_t y = 0; y < cropped_height; ++y) {
        cropped_matrix[y] = new uint64_t[cropped_width];
        for (uint64_t x = 0; x < cropped_width; ++x) {
            cropped_matrix[y][x] = matrix[min_y + y][min_x + x];
        }
    }

    writeBMP(cropped_width, cropped_height, cropped_matrix, filename, output_file);

    for (uint64_t y = 0; y < cropped_height; ++y) {
        delete[] cropped_matrix[y];
    }
    delete[] cropped_matrix;
}
