# Loop test from Zach's input_file.asm
# Counts $1 from 0 to 10, accumulating sum into $3
# $3 = 0+1+2+...+9 = 45

ADDI $1 $0 0      # $1 = 0  (counter)
ADDI $2 $0 10     # $2 = 10 (limit)
NOP
NOP
NOP
loop:
BEQ $1 $2 done    # if $1 == $2, exit loop
NOP
NOP
ADD $3 $3 $1      # $3 += $1
NOP
NOP
NOP
ADDI $1 $1 1      # $1++
NOP
NOP
NOP
J loop
NOP
NOP
done:
NOP

# Expected results:
# $1 = 10
# $2 = 10
# $3 = 45  (0+1+2+3+4+5+6+7+8+9)
