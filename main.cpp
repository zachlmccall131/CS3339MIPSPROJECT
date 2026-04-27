#include "mips.h"
#include "parser.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Usage: mips_sim <program.asm> [--debug]\n";
        return 1;
    }

    for (int i = 2; i < argc; i++)
        if (std::string(argv[i]) == "--debug") debugMode = true;

    std::vector<Instruction> program = parseProgram(argv[1]);

    int cycle = 0;
    while (PC < (int)program.size() ||
           !if_id.instr.opcode.empty()  ||
           !id_ex.instr.opcode.empty()  ||
           !ex_mem.instr.opcode.empty() ||
           !mem_wb.instr.opcode.empty()) {

        writeBack();
        memAccess();
        execute();
        decode();
        fetch(program);
        cycle++;

        if (debugMode) printState(cycle);
    }

    printFinal();
    return 0;
}
