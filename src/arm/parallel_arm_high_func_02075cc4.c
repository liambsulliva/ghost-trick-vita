asm unsigned int func_02075cc4(void)
{
    mrs r0, cpsr
    orr r1, r0, #192
    msr CPSR_c, r1
    and r0, r0, #192
    bx lr
}
