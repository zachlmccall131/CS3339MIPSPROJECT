#include "parser.h"
#include <fstream>
#include <sstream>
#include <unordered_map>

std::vector<Instruction> parseProgram(const std::string& filename) {
    std::ifstream file(filename);
    std::vector<std::string> lines;           // cleaned lines of assembly code
    std::unordered_map<std::string, int> labels; // maps label name to instruction index
    std::string line;

    // ── First Pass ─────────────────────────────────────────────────────────
    // Read each line, strip comments and blank lines, record any labels found
    while (std::getline(file, line)) {

        // Replace commas with spaces so "ADD $1, $2, $3" tokenizes the same as "ADD $1 $2 $3"
        for (char& c : line) if (c == ',') c = ' ';

        // Remove anything after a # comment
        if (line.find('#') != std::string::npos)
            line = line.substr(0, line.find('#'));

        // Skip blank lines
        if (line.find_first_not_of(" \t\r\n") == std::string::npos) continue;

        // If this line has a label (e.g. "loop:"), record its instruction index
        // and strip the label from the line before saving it
        if (line.find(':') != std::string::npos) {
            labels[line.substr(0, line.find(':'))] = (int)lines.size();
            line = line.substr(line.find(':') + 1);
            if (line.find_first_not_of(" \t\r\n") == std::string::npos) continue;
        }

        lines.push_back(line);
    }

    // ── Second Pass ────────────────────────────────────────────────────────
    // Parse each cleaned line into an Instruction struct
    std::vector<Instruction> program;
    for (int i = 0; i < (int)lines.size(); i++) {

        // Split the line into tokens by whitespace
        std::istringstream ss(lines[i]);
        std::vector<std::string> t;
        std::string tok;
        while (ss >> tok) t.push_back(tok);

        Instruction instr;
        instr.opcode = t[0]; // first token is always the opcode

        // Strips the $ from a register token and returns the number
        // e.g. "$5" -> 5
        auto reg = [](const std::string& r) { return std::stoi(r.substr(1)); };

        // R-type: ADD rd, rs, rt
        if (instr.opcode == "ADD" || instr.opcode == "SUB" ||
            instr.opcode == "MUL" || instr.opcode == "AND" || instr.opcode == "OR") {
            instr.rd = reg(t[1]); instr.rs = reg(t[2]); instr.rt = reg(t[3]);

        // Shift: SLL/SRL rd, rt, shamt  (shamt is a number, not a register)
        } else if (instr.opcode == "SLL" || instr.opcode == "SRL") {
            instr.rd = reg(t[1]); instr.rt = reg(t[2]); instr.shamt = std::stoi(t[3]);

        // I-type: ADDI rt, rs, immediate
        } else if (instr.opcode == "ADDI") {
            instr.rt = reg(t[1]); instr.rs = reg(t[2]); instr.immediate = std::stoi(t[3]);

        // Memory: LW/SW rt, offset(rs)  -- parse the offset and base register out of "offset(rs)"
        } else if (instr.opcode == "LW" || instr.opcode == "SW") {
            instr.rt        = reg(t[1]);
            instr.immediate = std::stoi(t[2].substr(0, t[2].find('(')));
            instr.rs        = reg(t[2].substr(t[2].find('(') + 1, t[2].find(')') - t[2].find('(') - 1));

        // Branch: BEQ rs, rt, label
        // Store the offset as (target index - next instruction index) so execute can add it to PC
        } else if (instr.opcode == "BEQ") {
            instr.rs = reg(t[1]); instr.rt = reg(t[2]);
            instr.immediate = labels.count(t[3]) ? labels[t[3]] - (i + 1) : std::stoi(t[3]);

        // Jump: J label  -- store the target instruction index directly
        } else if (instr.opcode == "J") {
            instr.address = labels.count(t[1]) ? labels[t[1]] : std::stoi(t[1]);
        }
        // NOP has no operands so nothing extra to parse

        program.push_back(instr);
    }
    return program;
}
