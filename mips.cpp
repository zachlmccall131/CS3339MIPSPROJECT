#include "mips.h"
#include <iomanip>

// ── Globals ────────────────────────────────────────────────────────────────

int registers[NUM_REGISTERS] = {0};
int memory[MEMORY_SIZE]      = {0};
int PC                       = 0;
bool debugMode               = false;

IF_ID  if_id;
ID_EX  id_ex;
EX_MEM ex_mem;
MEM_WB mem_wb;

// ── ALU ───────────────────────────────────────────────────────────────────
// ALUOp: 0=ADD, 1=SUB, 2=MUL, 3=AND, 4=OR, 5=SLL, 6=SRL

int alu(int a, int b, int aluOp) {
    switch (aluOp) {
        case 0: return a + b;
        case 1: return a - b;
        case 2: return a * b;
        case 3: return a & b;
        case 4: return a | b;
        case 5: return b << a;
        case 6: return (int)((unsigned)b >> a);
        default: return 0;
    }
}

// ── Control Unit ───────────────────────────────────────────────────────────

ControlSignals decodeControl(const std::string& opcode) {
    ControlSignals ctrl;

    if (opcode == "ADD") {
        ctrl.RegWrite = true; ctrl.ALUOp = 0;
    } else if (opcode == "ADDI") {
        ctrl.RegWrite = true; ctrl.ALUSrc = true; ctrl.ALUOp = 0;
    } else if (opcode == "SUB") {
        ctrl.RegWrite = true; ctrl.ALUOp = 1;
    } else if (opcode == "MUL") {
        ctrl.RegWrite = true; ctrl.ALUOp = 2;
    } else if (opcode == "AND") {
        ctrl.RegWrite = true; ctrl.ALUOp = 3;
    } else if (opcode == "OR") {
        ctrl.RegWrite = true; ctrl.ALUOp = 4;
    } else if (opcode == "SLL") {
        ctrl.RegWrite = true; ctrl.ALUSrc = true; ctrl.ALUOp = 5;
    } else if (opcode == "SRL") {
        ctrl.RegWrite = true; ctrl.ALUSrc = true; ctrl.ALUOp = 6;
    } else if (opcode == "LW") {
        ctrl.RegWrite = true; ctrl.MemRead = true;
        ctrl.MemToReg = true; ctrl.ALUSrc  = true; ctrl.ALUOp = 0;
    } else if (opcode == "SW") {
        ctrl.MemWrite = true; ctrl.ALUSrc = true; ctrl.ALUOp = 0;
    } else if (opcode == "BEQ") {
        ctrl.Branch = true; ctrl.ALUOp = 1;
    } else if (opcode == "J") {
        ctrl.Jump = true;
    }
    // NOP: all signals remain false/0

    return ctrl;
}

// ── Fetch ──────────────────────────────────────────────────────────────────

void fetch(const std::vector<Instruction>& program) {
    if (PC < (int)program.size()) {
        if_id.instr = program[PC];
        if_id.pc    = PC;
        PC++;
    } else {
        if_id = IF_ID();
    }
}

// ── Decode ─────────────────────────────────────────────────────────────────

void decode() {
    if (if_id.instr.opcode.empty()) { id_ex = ID_EX(); return; }

    id_ex.instr     = if_id.instr;
    id_ex.pc        = if_id.pc;
    id_ex.ctrl      = decodeControl(if_id.instr.opcode);
    id_ex.readData1 = registers[if_id.instr.rs];
    id_ex.readData2 = registers[if_id.instr.rt];
}

// ── Execute ────────────────────────────────────────────────────────────────

void execute() {
    if (id_ex.instr.opcode.empty()) { ex_mem = EX_MEM(); return; }

    ex_mem.instr     = id_ex.instr;
    ex_mem.ctrl      = id_ex.ctrl;
    ex_mem.writeData = id_ex.readData2;

    int aluA = id_ex.readData1;
    int aluB = id_ex.ctrl.ALUSrc ? id_ex.instr.immediate : id_ex.readData2;

    if (id_ex.instr.opcode == "SLL" || id_ex.instr.opcode == "SRL") {
        aluA = id_ex.instr.shamt;
        aluB = id_ex.readData2;
    }

    ex_mem.aluResult = alu(aluA, aluB, id_ex.ctrl.ALUOp);

    if (id_ex.instr.opcode == "ADD" || id_ex.instr.opcode == "SUB" ||
        id_ex.instr.opcode == "MUL" || id_ex.instr.opcode == "AND" ||
        id_ex.instr.opcode == "OR"  || id_ex.instr.opcode == "SLL" ||
        id_ex.instr.opcode == "SRL") {
        ex_mem.destReg = id_ex.instr.rd;
    } else {
        ex_mem.destReg = id_ex.instr.rt;
    }

    if (id_ex.ctrl.Branch && ex_mem.aluResult == 0)
        PC = id_ex.pc + 1 + id_ex.instr.immediate;

    if (id_ex.ctrl.Jump)
        PC = id_ex.instr.address;
}

// ── Memory Access ──────────────────────────────────────────────────────────

void memAccess() {
    if (ex_mem.instr.opcode.empty()) { mem_wb = MEM_WB(); return; }

    mem_wb.instr     = ex_mem.instr;
    mem_wb.ctrl      = ex_mem.ctrl;
    mem_wb.aluResult = ex_mem.aluResult;
    mem_wb.destReg   = ex_mem.destReg;

    if (ex_mem.ctrl.MemRead)
        mem_wb.memData = memory[ex_mem.aluResult];

    if (ex_mem.ctrl.MemWrite)
        memory[ex_mem.aluResult] = ex_mem.writeData;
}

// ── Write Back ─────────────────────────────────────────────────────────────

void writeBack() {
    if (mem_wb.instr.opcode.empty()) return;

    if (mem_wb.ctrl.RegWrite && mem_wb.destReg != 0) {
        if (mem_wb.ctrl.MemToReg)
            registers[mem_wb.destReg] = mem_wb.memData;
        else
            registers[mem_wb.destReg] = mem_wb.aluResult;
    }
}

// ── Debug Print ────────────────────────────────────────────────────────────

void printState(int cycle) {
    std::cout << "\n=== Cycle " << cycle << " ===\n";

    std::cout << "Registers:\n";
    for (int i = 0; i < NUM_REGISTERS; i++) {
        std::cout << "  $" << std::setw(2) << i << " = " << std::setw(6) << registers[i];
        if ((i + 1) % 4 == 0) std::cout << "\n";
    }

    std::cout << "IF/ID:  " << if_id.instr.opcode  << "  PC=" << if_id.pc << "\n";
    std::cout << "ID/EX:  " << id_ex.instr.opcode  << "  rd1=" << id_ex.readData1 << "  rd2=" << id_ex.readData2 << "\n";
    std::cout << "EX/MEM: " << ex_mem.instr.opcode << "  aluResult=" << ex_mem.aluResult << "  dest=$" << ex_mem.destReg << "\n";
    std::cout << "MEM/WB: " << mem_wb.instr.opcode << "  aluResult=" << mem_wb.aluResult << "  memData=" << mem_wb.memData << "\n";

    std::cout << "Control (ID/EX): RegWrite=" << id_ex.ctrl.RegWrite
              << " MemRead="  << id_ex.ctrl.MemRead
              << " MemWrite=" << id_ex.ctrl.MemWrite
              << " MemToReg=" << id_ex.ctrl.MemToReg
              << " ALUSrc="   << id_ex.ctrl.ALUSrc
              << " Branch="   << id_ex.ctrl.Branch
              << " Jump="     << id_ex.ctrl.Jump
              << " ALUOp="    << id_ex.ctrl.ALUOp << "\n";
}

// ── Final Output ───────────────────────────────────────────────────────────

void printFinal() {
    std::cout << "\n=== Final Register State ===\n";
    for (int i = 0; i < NUM_REGISTERS; i++) {
        std::cout << "  $" << std::setw(2) << i << " = " << std::setw(6) << registers[i];
        if ((i + 1) % 4 == 0) std::cout << "\n";
    }

    std::cout << "\n=== Final Memory State (non-zero) ===\n";
    for (int i = 0; i < MEMORY_SIZE; i++) {
        if (memory[i] != 0)
            std::cout << "  mem[" << i << "] = " << memory[i] << "\n";
    }
}
