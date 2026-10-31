asm void func_020751b4(register void *arg)
{
    ldr r1, =0x020d11dc
    str r0, [r1, #0x6c]
    ldr r0, [r12]
    str r0, [r1, #0x4]
    ldr r0, [r12, #0x4]
    str r0, [r1, #0x8]
    ldr r0, [r12, #0x8]
    str r0, [r1, #0xc]
    ldr r0, [r12, #0xc]
    str r0, [r1, #0x10]
    ldr r2, [r12, #0x10]
    bic r2, r2, #1
    add r0, r1, #20
    stmia r0, {r4, r5, r6, r7, r8, r9, r10, r11}
    ldr r0, [r2]
    str r0, [r1, #0x64]
    ldr r3, [r2, #0x4]
    str r3, [r1]
    ldr r0, [r2, #0x8]
    str r0, [r1, #0x34]
    ldr r0, [r2, #0xc]
    str r0, [r1, #0x40]
    mrs r0, cpsr
    orr r3, r3, #128
    bic r3, r3, #32
    msr CPSR_fsxc, r3
    str sp, [r1, #0x38]
    str lr, [r1, #0x3c]
    mrs r2, spsr
    msr CPSR_fsxc, r0
    bx lr
}
