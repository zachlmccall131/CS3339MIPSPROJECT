# Comprehensive test — uses every instruction

ADDI $1, $0, 10   # $1 = 10
ADDI $2, $0, 4    # $2 = 4
NOP
NOP
NOP
ADD  $3, $1, $2   # $3 = 14
NOP
NOP
NOP
SUB  $4, $1, $2   # $4 = 6
NOP
NOP
NOP
MUL  $5, $1, $2   # $5 = 40
NOP
NOP
NOP
AND  $6, $1, $2   # $6 = 0  (1010 & 0100 = 0000)
NOP
NOP
NOP
OR   $7, $1, $2   # $7 = 14 (1010 | 0100 = 1110)
NOP
NOP
NOP
SLL  $8, $2, 2    # $8 = 16 (4 << 2)
NOP
NOP
NOP
SRL  $9, $1, 1    # $9 = 5  (10 >> 1)
NOP
NOP
NOP
SW   $1, 0($0)    # mem[0] = 10
NOP
NOP
NOP
LW   $10, 0($0)   # $10 = 10
NOP
NOP
NOP
BEQ  $1, $10, skip # branch taken (both == 10)
NOP
NOP
ADDI $11, $0, 99  # skipped
skip:
J done             # jump to done
NOP
NOP
ADDI $12, $0, 99  # skipped
done:
ADDI $13, $0, 1   # $13 = 1

# Expected results:
# $1=10, $2=4,  $3=14, $4=6,  $5=40
# $6=0,  $7=14, $8=16, $9=5,  $10=10
# $11=0 (skipped), $12=0 (skipped), $13=1
# mem[0] = 10
