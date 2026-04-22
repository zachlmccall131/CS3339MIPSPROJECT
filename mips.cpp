#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "/Users/zachmccall/Documents/school_work/spring_2026/comp_arch/projects/CS3339MIPSPROJECT/mips.h"

std::ifstream fin;

//Register File
int registers[NUM_REGISTERS] = {0};

//Memory 
int memory[MEMORY_SIZE] = {0};

//Program Counter 
int PC = 0;

//Pipeline State 
IF_ID  if_id;
ID_EX  id_ex;
EX_MEM ex_mem;
MEM_WB mem_wb;

void fetch(){
    // Input an assembly instruction from file
    std::string input;
    if (std::getline(fin, input)) {
        // Parse the input (simple parser for demonstration)
        // This is a basic parser; you may need to expand it for all instructions
        std::istringstream iss(input);
        std::string opcode;
        iss >> opcode;

        if_id.instr.opcode = opcode;
        if_id.pc = PC;

        if (opcode == "ADD" || opcode == "SUB" || opcode == "MUL" || opcode == "AND" || opcode == "OR") {
            // R-type: opcode rd, rs, rt
            std::string rd_str, rs_str, rt_str;
            iss >> rd_str >> rs_str >> rt_str;
            // Remove $ and convert to int
            if_id.instr.rd = std::stoi(rd_str.substr(1));
            if_id.instr.rs = std::stoi(rs_str.substr(1));
            if_id.instr.rt = std::stoi(rt_str.substr(1));
        } else if (opcode == "ADDI" || opcode == "LW" || opcode == "SW") {
            // I-type: opcode rt, rs, immediate
            std::string rt_str, rs_str, imm_str;
            iss >> rt_str >> rs_str >> imm_str;
            if_id.instr.rt = std::stoi(rt_str.substr(1));
            if_id.instr.rs = std::stoi(rs_str.substr(1));
            if_id.instr.immediate = std::stoi(imm_str);
        } else if (opcode == "BEQ") {
            // Branch: opcode rs, rt, label (simplified to immediate)
            std::string rs_str, rt_str, imm_str;
            iss >> rs_str >> rt_str >> imm_str;
            if_id.instr.rs = std::stoi(rs_str.substr(1));
            if_id.instr.rt = std::stoi(rt_str.substr(1));
            if_id.instr.immediate = std::stoi(imm_str);
        } else if (opcode == "J") {
            // Jump: opcode address
            std::string addr_str;
            iss >> addr_str;
            if_id.instr.address = std::stoi(addr_str);
        } else if (opcode == "SLL" || opcode == "SRL") {
            // Shift: opcode rd, rt, shamt
            std::string rd_str, rt_str, shamt_str;
            iss >> rd_str >> rt_str >> shamt_str;
            if_id.instr.rd = std::stoi(rd_str.substr(1));
            if_id.instr.rt = std::stoi(rt_str.substr(1));
            if_id.instr.shamt = std::stoi(shamt_str);
        } else if (opcode == "NOP") {
            // No operands
        } else {
            std::cerr << "Unknown opcode: " << opcode << std::endl;
        }

        // Update PC (assuming 4-byte instructions)
        PC += 4;
    } else {
        // End of file, perhaps set to NOP or handle end
        if_id.instr.opcode = "NOP";
        if_id.pc = PC;
    }
}
void decode(){
    // Read registers
    id_ex.readData1 = registers[if_id.instr.rs];
    id_ex.readData2 = registers[if_id.instr.rt];

    if (if_id.instr.opcode == "ADD") {
        // Example instruction, replace with actual decoding logic
        id_ex.ctrl.RegWrite = true;
        id_ex.instr = if_id.instr;
        id_ex.pc = if_id.pc;
    }
    else if (if_id.instr.opcode == "LW") {
        // Set control signals for LW
        id_ex.ctrl.MemRead = true;
        id_ex.ctrl.MemToReg = true;
        id_ex.ctrl.RegWrite = true;
        id_ex.instr = if_id.instr;
        id_ex.pc = if_id.pc;
    }
    else if (if_id.instr.opcode == "SW") {
        // Set control signals for SW
        id_ex.ctrl.MemWrite = true;
        id_ex.instr = if_id.instr;
        id_ex.pc = if_id.pc;
    }
    else if (if_id.instr.opcode == "BEQ") {
        // Set control signals for BEQ
        id_ex.ctrl.Branch = true;
        id_ex.instr = if_id.instr;
        id_ex.pc = if_id.pc;
    }
    else if (if_id.instr.opcode == "J") {
        // Set control signals for J
        id_ex.ctrl.Jump = true;
        id_ex.instr = if_id.instr;
        id_ex.pc = if_id.pc;
    }
    else if (if_id.instr.opcode == "NOP") {
        // Set control signals for NOP
        id_ex.instr = if_id.instr;
        id_ex.pc = if_id.pc;
    }       
    else if(if_id.instr.opcode == "ADDI") {
        // Set control signals for ADDI
        id_ex.ctrl.ALUSrc = true;
        id_ex.ctrl.RegWrite = true;
        id_ex.instr = if_id.instr;
        id_ex.pc = if_id.pc;
    }
    else if(if_id.instr.opcode == "SUB") {
        // Set control signals for SUB
        id_ex.ctrl.RegWrite = true;
        id_ex.instr = if_id.instr;
        id_ex.pc = if_id.pc;
    }
    else if (if_id.instr.opcode == "MUL") {
        // Set control signals for MUL
        id_ex.ctrl.RegWrite = true;
        id_ex.instr = if_id.instr;
        id_ex.pc = if_id.pc;
    }
    else if (if_id.instr.opcode == "AND") {
        // Set control signals for AND
        id_ex.ctrl.RegWrite = true;
        id_ex.instr = if_id.instr;
        id_ex.pc = if_id.pc;
    }
    else if (if_id.instr.opcode == "OR") {
        // Set control signals for OR
        id_ex.ctrl.RegWrite = true;
        id_ex.instr = if_id.instr;
        id_ex.pc = if_id.pc;
    }
    else if (if_id.instr.opcode == "SLL") {
        // Set control signals for SLL
        id_ex.ctrl.ALUSrc = true;
        id_ex.ctrl.RegWrite = true;
        id_ex.instr = if_id.instr;
        id_ex.pc = if_id.pc;
    }
    else if (if_id.instr.opcode == "SRL") {
        // Set control signals for SRL
        id_ex.ctrl.ALUSrc = true;
        id_ex.ctrl.RegWrite = true;
        id_ex.instr = if_id.instr;
        id_ex.pc = if_id.pc;
    }
    else {
        // Handle other instructions or NOP
        std::cerr << "Unknown instruction: " << if_id.instr.opcode << std::endl;
    }
    //decodes the instruction and sets control signals
}
void execute(){
    // Transfer instruction and control signals to EX/MEM
    ex_mem.instr = id_ex.instr;
    ex_mem.ctrl = id_ex.ctrl;
    ex_mem.writeData = id_ex.readData2; // For SW instruction

    // Performs ALU operations and calculates branch target
    if(id_ex.instr.opcode == "ADD") {
        ex_mem.aluResult = id_ex.readData1 + id_ex.readData2;
        ex_mem.destReg = id_ex.instr.rd;
    }
    else if (id_ex.instr.opcode == "ADDI") {
        ex_mem.aluResult = id_ex.readData1 + id_ex.instr.immediate;
        ex_mem.destReg = id_ex.instr.rt;
    }
    else if (id_ex.instr.opcode == "SUB") {
        ex_mem.aluResult = id_ex.readData1 - id_ex.readData2;
        ex_mem.destReg = id_ex.instr.rd;
    }
    else if (id_ex.instr.opcode == "MUL") {
        ex_mem.aluResult = id_ex.readData1 * id_ex.readData2;
        ex_mem.destReg = id_ex.instr.rd;
    }
    else if (id_ex.instr.opcode == "AND") {
        ex_mem.aluResult = id_ex.readData1 & id_ex.readData2;
        ex_mem.destReg = id_ex.instr.rd;
    }
    else if (id_ex.instr.opcode == "OR") {
        ex_mem.aluResult = id_ex.readData1 | id_ex.readData2;
        ex_mem.destReg = id_ex.instr.rd;
    }
    else if (id_ex.instr.opcode == "SLL") {
        ex_mem.aluResult = id_ex.readData1 << id_ex.instr.shamt;
        ex_mem.destReg = id_ex.instr.rd;
    }
    else if (id_ex.instr.opcode == "SRL") {
        ex_mem.aluResult = static_cast<unsigned int>(id_ex.readData1) >> id_ex.instr.shamt;
        ex_mem.destReg = id_ex.instr.rd;
    }
    else if (id_ex.ctrl.Jump) {
        // Calculate jump target
        ex_mem.aluResult = (id_ex.pc & 0xF0000000) | (id_ex.instr.address << 2); // Example jump target calculation
    }
    else if( id_ex.instr.opcode == "NOP") {
        // No operation, do nothing
    }
    else if (id_ex.instr.opcode == "LW" || id_ex.instr.opcode == "SW") {
        ex_mem.aluResult = id_ex.readData1 + id_ex.instr.immediate; // Calculate memory address
        ex_mem.writeData = id_ex.readData2; // Data to write for SW
        ex_mem.destReg = id_ex.instr.rt; // Destination register for LW
    }
    else if (id_ex.instr.opcode == "BEQ") {
        // Set branch taken flag if condition is met
        if (id_ex.readData1 == id_ex.readData2) {
            ex_mem.aluResult = id_ex.pc + (id_ex.instr.immediate << 2); // Example branch target calculation
        }
    }
}
void memoryAccess(){
    // Transfer data from EX/MEM to MEM/WB
    mem_wb.instr = ex_mem.instr;
    mem_wb.ctrl = ex_mem.ctrl;
    mem_wb.aluResult = ex_mem.aluResult;
    mem_wb.destReg = ex_mem.destReg;

    // Perform memory operations based on control signals
    if (ex_mem.ctrl.MemRead) {
        // Read from memory at the address calculated in execute
        mem_wb.memData = memory[ex_mem.aluResult];
    } else if (ex_mem.ctrl.MemWrite) {
        // Write to memory at the address calculated in execute
        memory[ex_mem.aluResult] = ex_mem.writeData;
    }
    // For other instructions, memData remains unchanged
}
void writeBack(){
    // Write data back to the register file based on control signals
    if (mem_wb.ctrl.RegWrite) {
        if (mem_wb.ctrl.MemToReg) {
            // Write memory data to register (for LW)
            registers[mem_wb.destReg] = mem_wb.memData;
        } else {
            // Write ALU result to register (for R-type and other instructions)
            registers[mem_wb.destReg] = mem_wb.aluResult;
        }
    }
}
void printState(){
    //prints the current state of the processor
    std::cout << "PC: " << if_id.pc << std::endl;
    std::cout << "IF/ID: " << if_id.instr.opcode << ", PC: " << if_id.pc << std::endl;
    std::cout << "ID/EX: " << id_ex.instr.opcode << ", PC: " << id_ex.pc << std::endl;
    std::cout << "EX/MEM: " << ex_mem.instr.opcode << ", ALU Result: " << ex_mem.aluResult << std::endl;
    std::cout << "MEM/WB: " << mem_wb.instr.opcode << ", ALU Result: " << mem_wb.aluResult << std::endl;

}
