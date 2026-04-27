# Tests: BEQ
# Branch should be taken (5 == 5), skipping ADDI $3

ADDI $1, $0, 5    # $1 = 5
ADDI $2, $0, 5    # $2 = 5
NOP
NOP
NOP
BEQ $1, $2, done  # branch taken, skip next instruction
NOP
NOP
ADDI $3, $0, 99   # should be skipped
done:
ADDI $4, $0, 1    # $4 = 1

# Expected results:
# $1 = 5
# $2 = 5
# $3 = 0  (skipped)
# $4 = 1
