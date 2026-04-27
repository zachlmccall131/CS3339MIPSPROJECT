# Tests: SW, LW

ADDI $1, $0, 42   # $1 = 42
ADDI $2, $0, 7    # $2 = 7
NOP
NOP
NOP
SW $1, 0($0)      # mem[0] = 42
SW $2, 1($0)      # mem[1] = 7
NOP
NOP
NOP
LW $3, 0($0)      # $3 = 42
NOP
NOP
NOP
LW $4, 1($0)      # $4 = 7

# Expected results:
# $3 = 42
# $4 = 7
# mem[0] = 42
# mem[1] = 7
