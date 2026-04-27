#pragma once
#include "mips.h"
#include <vector>
#include <string>

std::vector<Instruction> parseProgram(const std::string& filename);
