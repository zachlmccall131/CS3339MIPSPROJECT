#include "parser.h"
#include <fstream>
#include <sstream>
#include <unordered_map>

std::vector<Instruction> parseProgram(const std::string& filename) {
    std::ifstream file(filename);
    std::vector<std::string> lines;
    std::unordered_map<std::string, int> labels;
    std::string line;

    // First pass: collect labels and lines
    while (std::getline(file, line)) {
        for (char& c : line) if (c == ',') c = ' ';
        auto comment = line.find('#');
        if (comment != std::string::npos) line = line.substr(0, comment);
        if (line.find_first_not_of(" \t\r\n") == std::string::npos) continue;

        auto colon = line.find(':');
        if (colon != std::string::npos) {
            labels[line.substr(0, colon)] = (int)lines.size();
            line = line.substr(colon + 1);
        }
        if (line.find_first_not_of(" \t\r\n") == std::string::npos) continue;
        lines.push_back(line);
    }

    // Second pass: parse instructions
    std::vector<Instruction> program;
    for (int i = 0; i < (int)lines.size(); i++) {
        std::istringstream ss(lines[i]);
        std::vector<std::string> t;
        std::string tok;
        while (ss >> tok) t.push_back(tok);

        Instruction instr;
        instr.opcode = t[0];

        auto reg = [](const std::string& r) -> int {
            // Named registers
            if (r == "$zero") return 0;
            if (r == "$at")   return 1;
            if (r == "$v0")   return 2;  if (r == "$v1") return 3;
            if (r == "$a0")   return 4;  if (r == "$a1") return 5;
            if (r == "$a2")   return 6;  if (r == "$a3") return 7;
            if (r == "$t0")   return 8;  if (r == "$t1") return 9;
            if (r == "$t2")   return 10; if (r == "$t3") return 11;
            if (r == "$t4")   return 12; if (r == "$t5") return 13;
            if (r == "$t6")   return 14; if (r == "$t7") return 15;
            if (r == "$s0")   return 16; if (r == "$s1") return 17;
            if (r == "$s2")   return 18; if (r == "$s3") return 19;
            if (r == "$s4")   return 20; if (r == "$s5") return 21;
            if (r == "$s6")   return 22; if (r == "$s7") return 23;
            if (r == "$t8")   return 24; if (r == "$t9") return 25;
            if (r == "$gp")   return 28; if (r == "$sp") return 29;
            if (r == "$fp")   return 30; if (r == "$ra") return 31;
            // Numeric: $0-$31
            return std::stoi(r.substr(1));
        };

        if (instr.opcode == "ADD" || instr.opcode == "SUB" ||
            instr.opcode == "MUL" || instr.opcode == "AND" || instr.opcode == "OR") {
            instr.rd = reg(t[1]); instr.rs = reg(t[2]); instr.rt = reg(t[3]);

        } else if (instr.opcode == "SLL" || instr.opcode == "SRL") {
            instr.rd = reg(t[1]); instr.rt = reg(t[2]); instr.shamt = std::stoi(t[3]);

        } else if (instr.opcode == "ADDI") {
            instr.rt = reg(t[1]); instr.rs = reg(t[2]); instr.immediate = std::stoi(t[3]);

        } else if (instr.opcode == "LW" || instr.opcode == "SW") {
            instr.rt = reg(t[1]);
            auto lp = t[2].find('(');
            auto rp = t[2].find(')');
            instr.immediate = std::stoi(t[2].substr(0, lp));
            instr.rs        = reg(t[2].substr(lp + 1, rp - lp - 1));

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
