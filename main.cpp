#include <iostream>
#include <string>
#include <fstream>
#include "/Users/zachmccall/Documents/school_work/spring_2026/comp_arch/projects/CS3339MIPSPROJECT/mips.h"

int main() {
    // Open the input file
    fin.open("input_file.asm");
    if (!fin.is_open()) {
        std::cerr << "Error opening input_file.asm" << std::endl;
        return 1;
    }

    // Initialize some basic setup if needed
    // For example, load instructions into memory or set initial PC

    // Simulate a few pipeline cycles
    const int NUM_CYCLES = 10;  // Adjust as needed

    for (int cycle = 0; cycle < NUM_CYCLES; ++cycle) {
        std::cout << "Cycle " << cycle + 1 << ":" << std::endl;

        fetch();
        
        decode();
        execute();
        memoryAccess();
        writeBack();

        printState();
        std::cout << std::endl;
    }

    fin.close();
    return 0;
}