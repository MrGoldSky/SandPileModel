#include <iostream>

#include "lib/getdata.h"
#include "lib/imagegenerator.h"
#include "lib/parser.h"
#include "lib/sandpile.h"

int main(int argc, char* argv[]) {
    try {
        Parser parser(argc, argv);
        parser.Print();
        Data data_from_tsv = GetDataFromTSV(parser);

        SandPile sandpile(parser, data_from_tsv);
        uint64_t count = 1;

        while (!sandpile.is_stable && sandpile.iterations < parser.max_iteration) {
            sandpile.Collapse();
            if (parser.frequency && (++count % parser.frequency == 0)) {
                sandpile.SaveToBMP(count, parser.output_file);
            }
        }
        // sandpile.PrintMatrix();
        sandpile.SaveToBMP(0, parser.output_file);

        if (!sandpile.CheckStable())
            std::cout << "The files are saved, but the pile is not completely scattered.." << '\n';
        else
            std::cout << "Saved successfully." << '\n';
    } catch (std::invalid_argument& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
