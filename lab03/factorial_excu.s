.globl factorial

.data
n: .word 8

.text
main:
    la t0, n
    lw a0, 0(t0)
    jal ra, factorial

    addi a1, a0, 0
    addi a0, x0, 1
    ecall # Print Result

    addi a1, x0, '\n'
    addi a0, x0, 11
    ecall # Print newline

    addi a0, x0, 10
    ecall # Exit

factorial:
    # YOUR CODE HERE
    addi sp, sp, -8
    sw a0, 0(sp)
    sw ra, 4(sp)
    beq a0, x0, ELSE
    addi a0, a0, -1
    jal ra, factorial
    lw t1, 0(sp)
    mul a0, a0, t1
    j ENDIF
ELSE:
    addi a0, x0, 1
ENDIF:
    lw ra,4(sp)
    addi sp, sp, 8
    jr ra
    