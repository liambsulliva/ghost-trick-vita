asm unsigned int func_02075cd8(register unsigned int bits)
{
    mrs r1, cpsr
    bic r2, r1, #192
    orr r2, r2, r0
    msr CPSR_c, r2
    and r0, r1, #192
    bx lr
}
