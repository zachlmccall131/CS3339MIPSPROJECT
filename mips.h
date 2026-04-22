#pragma once
#include <cstdint>
#include <string>
#include <array>
#include <fstream>

//Constants
const int NUM_REGISTERS = 32;
const int MEMORY_SIZE   = 256;

// Input file stream
extern std::ifstream fin;

//Instruction Struct 
struct Instruction {
    std::string opcode;  //"ADD", "LW", "BEQ"
    int rs        = 0;
    int rt        = 0;
    int rd        = 0;
    int immediate = 0;
    int shamt     = 0;
    int address   = 0;
};

//Control Signals Struct 
struct ControlSignals {
    bool RegWrite = false;
    bool MemRead  = false;
    bool MemWrite = false;
    bool MemToReg = false;
    bool ALUSrc   = false;
    bool Branch   = false;
    bool Jump     = false;
    int  ALUOp    = 0;
};

//instruction readings
/************************************ 


| Instruction | RegWrite | MemRead | MemWrite | MemToReg | ALUSrc | Branch | Jump | ALUOp |
| ----------- | -------: | ------: | -------: | -------: | -----: | -----: | ---: | ----- |
| ADD         |        1 |       0 |        0 |        0 |      0 |      0 |    0 | ADD   |
| ADDI        |        1 |       0 |        0 |        0 |      1 |      0 |    0 | ADD   |
| SUB         |        1 |       0 |        0 |        0 |      0 |      0 |    0 | SUB   |
| MUL         |        1 |       0 |        0 |        0 |      0 |      0 |    0 | MUL   |
| AND         |        1 |       0 |        0 |        0 |      0 |      0 |    0 | AND   |
| OR          |        1 |       0 |        0 |        0 |      0 |      0 |    0 | OR    |
| SLL         |        1 |       0 |        0 |        0 |      1 |      0 |    0 | SLL   |
| SRL         |        1 |       0 |        0 |        0 |      1 |      0 |    0 | SRL   |
| LW          |        1 |       1 |        0 |        1 |      1 |      0 |    0 | ADD   |
| SW          |        0 |       0 |        1 |        0 |      1 |      0 |    0 | ADD   |
| BEQ         |        0 |       0 |        0 |        0 |      0 |      1 |    0 | SUB   |
| J           |        0 |       0 |        0 |        0 |      0 |      0 |    1 | NONE  |
| NOP         |        0 |       0 |        0 |        0 |      0 |      0 |    0 | NONE  |

*************************************/

//Pipeline Structs 

struct IF_ID {
    Instruction instr;
    int pc = 0;
};

struct ID_EX {
    Instruction    instr;
    ControlSignals ctrl;
    int readData1 = 0;
    int readData2 = 0;
    int pc        = 0;
};

struct EX_MEM {
    Instruction    instr;
    ControlSignals ctrl;
    int aluResult = 0;
    int writeData = 0;
    int destReg   = 0;
};

struct MEM_WB {
    Instruction    instr;
    ControlSignals ctrl;
    int aluResult = 0;
    int memData   = 0; 
    int destReg   = 0;
};

//Pipeline State 
// Defined in mips.cpp

//Function Prototypes
void fetch();
void decode();
void execute();
void memoryAccess();
void writeBack();
void printState();