asm unsigned int func_02075cac(register unsigned int bits)
{
    mrs r1, cpsr
    bic r2, r1, #128
    orr r2, r2, r0
    msr CPSR_c, r2
    and r0, r1, #128
    bx lr
}
