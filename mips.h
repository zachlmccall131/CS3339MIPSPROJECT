#pragma once
#include <cstdint>
#include <string>
#include <array>

//Constants
const int NUM_REGISTERS = 32;
const int MEMORY_SIZE   = 256;

//Register File
int registers[NUM_REGISTERS] = {0};

//Memory 
int memory[MEMORY_SIZE] = {0};

//Program Counter 
int PC = 0;

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
IF_ID  if_id;
ID_EX  id_ex;
EX_MEM ex_mem;
MEM_WB mem_wb;
