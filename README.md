# CS3339MIPSPROJECT
CS 3339 MIPS Project Spring 2026

The purpose of this project is to implement different MIPS instructions and demonstrate them through a 5-stage pipelined processor simulator.

## How to Compile

Make sure you have `g++` installed. From the project root directory, run:

```
g++ -o mips_sim main.cpp mips.cpp parser.cpp
```

## How to Run

```
./mips_sim <program.asm>
```

To run with debug mode (prints register file, pipeline latches, and control signals after each cycle):

```
./mips_sim <program.asm> --debug
```

## Example

```
./mips_sim tests/test_arithmetic.asm
./mips_sim tests/test_all.asm --debug
```

## Test Cases

All test programs are in the `tests/` folder:

| File | What it tests |
|------|--------------|
| test_arithmetic.asm | ADD, ADDI, SUB, MUL |
| test_logic.asm | AND, OR, SLL, SRL |
| test_memory.asm | LW, SW |
| test_branch.asm | BEQ |
| test_jump.asm | J |
| test_loop.asm | Loop using BEQ and J |
| test_all.asm | All instructions together |

## Supported Instructions

ADD, ADDI, SUB, MUL, AND, OR, SLL, SRL, LW, SW, BEQ, J, NOP
