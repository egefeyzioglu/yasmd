#include <iostream>
#include <format>

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
    
}