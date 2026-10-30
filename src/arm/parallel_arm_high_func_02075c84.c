asm unsigned int func_02075c84(void)
{
    mrs r0, cpsr
    bic r1, r0, #128
    msr CPSR_c, r1
    and r0, r0, #128
    bx lr
}
