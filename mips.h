#pragma once
#include <string>
#include <vector>
#include <iostream>

const int NUM_REGISTERS = 32;
const int MEMORY_SIZE   = 256;

// ── Structs ────────────────────────────────────────────────────────────────

struct Instruction {
    std::string opcode;
    int rs        = 0;
    int rt        = 0;
    int rd        = 0;
    int immediate = 0;
    int shamt     = 0;
    int address   = 0;
};

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

// ── Globals ────────────────────────────────────────────────────────────────

extern int registers[NUM_REGISTERS];
extern int memory[MEMORY_SIZE];
extern int PC;
extern bool debugMode;

extern IF_ID  if_id;
extern ID_EX  id_ex;
extern EX_MEM ex_mem;
extern MEM_WB mem_wb;

// ── Functions ──────────────────────────────────────────────────────────────

int            alu           (int a, int b, int aluOp);
ControlSignals decodeControl (const std::string& opcode);

void fetch      (const std::vector<Instruction>& program);
void decode     ();
void execute    ();
void memAccess  ();
void writeBack  ();
void printState (int cycle);
void printFinal ();
