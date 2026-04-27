# Tests: J
# Jump should skip over ADDI $2

ADDI $1, $0, 10   # $1 = 10
J done             # jump over next instruction
NOP
NOP
ADDI $2, $0, 99   # should be skipped
done:
ADDI $3, $0, 5    # $3 = 5

# Expected results:
# $1 = 10
# $2 = 0  (skipped)
# $3 = 5
