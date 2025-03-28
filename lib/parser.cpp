#include "parser.h"

// Проверка, что строка является числом
bool CheckDigit(const char* str) {
    int length = std::strlen(str);
    for (int i = 0; i < length; i++) {
        if (!std::isdigit(static_cast<unsigned char>(str[i]))) {
            throw std::invalid_argument("Invalid number argument");
        }
    }
    return true;
}

// Конструктор парсера
Parser::Parser(int argc, char** argv) {
    char* args[8];
    int count_args = 0;

    if (argc > 8)
        throw std::invalid_argument("To many arguments");

    // Парсинг аргументов
    for (int index_argument = 1; index_argument < argc; ++index_argument) {
        char* arg = new char[std::strlen(argv[index_argument]) + 1];
        std::strcpy(arg, argv[index_argument]);

        char* equal_sign = std::strchr(arg, '=');
        if (equal_sign != nullptr) {
            *equal_sign = '\0';
            args[count_args++] = arg;
            args[count_args++] = equal_sign + 1;
        } else {
            args[count_args++] = arg;
        }
    }

    // Сохраняем результаты парсинга
    bool skip_next = false;
    for (int i = 0; i < count_args; i++) {
        if (skip_next) {
            skip_next = false;
            continue;
        }
        if (std::strcmp(args[i], "--input") == 0 || std::strcmp(args[i], "-i") == 0) {
            input_file = args[i + 1];
            skip_next = true;
        } else if (std::strcmp(args[i], "--output") == 0 || std::strcmp(args[i], "-o") == 0) {
            output_file = args[i + 1];
            skip_next = true;
        } else if (std::strcmp(args[i], "--max-iter") == 0 || std::strcmp(args[i], "-m") == 0) {
            if (CheckDigit(args[i + 1])) {
                max_iteration = std::atoi(args[i + 1]);
                skip_next = true;
            }
        } else if (std::strcmp(args[i], "--freq") == 0 || std::strcmp(args[i], "-f") == 0) {
            if (CheckDigit(args[i + 1])) {
                frequency = std::atoi(args[i + 1]);
                skip_next = true;
            }
        } else {
            throw std::invalid_argument("Invalid argument " + std::string(args[i]));
        }
    }

    for (int i = 0; i < count_args; i += 2) {
        delete[] args[i];
    }
}

void Parser::Print() {
    std::cout << "Input file: " << input_file << std::endl;
    std::cout << "Output file: " << output_file << std::endl;
    std::cout << "Frequency: " << frequency << std::endl;
    std::cout << "Max iteration: " << max_iteration << std::endl;
}
