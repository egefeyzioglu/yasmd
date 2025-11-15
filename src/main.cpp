#include <iostream>
#include <fstream>
#include <format>
#include <vector>
#include <regex>

#include "token.hpp"

constexpr char usage[] = "Usage: {} infile outfile";

inline void print_usage(char *argv0){
    std::cerr << std::format(usage, argv0);
}

int main(int argc, char **argv){
    if (argc != 3)
    {
        print_usage(argv[0]);
        return EXIT_FAILURE;
    }
    
    std::ifstream infile(argv[1]);
    std::stringstream source_ss;
    source_ss << infile.rdbuf();
    std::cout << Tokenizer::tokenize(source_ss.str()) << std::endl;
}