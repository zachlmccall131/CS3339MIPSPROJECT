# CS3339MIPSPROJECT
CS 3339 MIPS Project Spring 2026

The purpose of this project is to implement different MIPS instructions and demonstrate them through a 5-stage pipelined processor simulator.

## How to Compile

Make sure you have g++ installed. From the project root directory, run:

g++ -std=c++11 -o mips_sim main.cpp mips.cpp parser.cpp

## How to Run

./mips_sim tests/program.asm

To run with debug mode which prints the register file, pipeline latches, and control signals after each cycle, run:

./mips_sim tests/program.asm --debug

## Supported Instructions

ADD, ADDI, SUB, MUL, AND, OR, SLL, SRL, LW, SW, BEQ, J, NOP
