# Tests: ADD, ADDI, SUB, MUL
# Load values first, then operate after 3 NOPs to avoid hazards

ADDI $1, $0, 10   # $1 = 10
ADDI $2, $0, 3    # $2 = 3
NOP
NOP
NOP
ADD $3, $1, $2    # $3 = 13
NOP
NOP
NOP
SUB $4, $1, $2    # $4 = 7
NOP
NOP
NOP
MUL $5, $1, $2    # $5 = 30

# Expected results:
# $1 = 10
# $2 = 3
# $3 = 13
# $4 = 7
# $5 = 30
