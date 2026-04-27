# Tests: AND, OR, SLL, SRL
# 12 = 1100, 10 = 1010

ADDI $1, $0, 12   # $1 = 12
ADDI $2, $0, 10   # $2 = 10
NOP
NOP
NOP
AND $3, $1, $2    # $3 = 8  (1100 & 1010 = 1000)
NOP
NOP
NOP
OR  $4, $1, $2    # $4 = 14 (1100 | 1010 = 1110)
NOP
NOP
NOP
SLL $5, $1, 2     # $5 = 48 (12 << 2)
NOP
NOP
NOP
SRL $6, $1, 2     # $6 = 3  (12 >> 2)

# Expected results:
# $1 = 12
# $2 = 10
# $3 = 8
# $4 = 14
# $5 = 48
# $6 = 3
