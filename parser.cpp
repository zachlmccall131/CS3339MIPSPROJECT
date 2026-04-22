#include "parser.h"
#include <fstream>
#include <sstream>
#include <unordered_map>

std::vector<Instruction> parseProgram(const std::string& filename) {
    std::ifstream file(filename);
    std::vector<std::string> lines;
    std::unordered_map<std::string, int> labels;
    std::string line;

    // First pass: collect labels and clean lines
    while (std::getline(file, line)) {
        for (char& c : line) if (c == ',') c = ' ';
        if (line.find('#') != std::string::npos) line = line.substr(0, line.find('#'));
        if (line.find_first_not_of(" \t\r\n") == std::string::npos) continue;

        if (line.find(':') != std::string::npos) {
            labels[line.substr(0, line.find(':'))] = (int)lines.size();
            line = line.substr(line.find(':') + 1);
            if (line.find_first_not_of(" \t\r\n") == std::string::npos) continue;
        }
        lines.push_back(line);
    }

    // Second pass: parse each line into an Instruction
    std::vector<Instruction> program;
    for (int i = 0; i < (int)lines.size(); i++) {
        std::istringstream ss(lines[i]);
        std::vector<std::string> t;
        std::string tok;
        while (ss >> tok) t.push_back(tok);

        Instruction instr;
        instr.opcode = t[0];

        // Converts "$5" -> 5
        auto reg = [](const std::string& r) { return std::stoi(r.substr(1)); };

        if (instr.opcode == "ADD" || instr.opcode == "SUB" ||
            instr.opcode == "MUL" || instr.opcode == "AND" || instr.opcode == "OR") {
            instr.rd = reg(t[1]); instr.rs = reg(t[2]); instr.rt = reg(t[3]);

        } else if (instr.opcode == "SLL" || instr.opcode == "SRL") {
            instr.rd = reg(t[1]); instr.rt = reg(t[2]); instr.shamt = std::stoi(t[3]);

        } else if (instr.opcode == "ADDI") {
            instr.rt = reg(t[1]); instr.rs = reg(t[2]); instr.immediate = std::stoi(t[3]);

        } else if (instr.opcode == "LW" || instr.opcode == "SW") {
            instr.rt = reg(t[1]);
            instr.immediate = std::stoi(t[2].substr(0, t[2].find('(')));
            instr.rs = reg(t[2].substr(t[2].find('(') + 1, t[2].find(')') - t[2].find('(') - 1));

        } else if (instr.opcode == "BEQ") {
            instr.rs = reg(t[1]); instr.rt = reg(t[2]);
            instr.immediate = labels.count(t[3]) ? labels[t[3]] - (i + 1) : std::stoi(t[3]);

        } else if (instr.opcode == "J") {
            instr.address = labels.count(t[1]) ? labels[t[1]] : std::stoi(t[1]);
        }

        program.push_back(instr);
    }
    return program;
}
