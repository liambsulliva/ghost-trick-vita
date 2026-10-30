asm void func_020744bc(register int context)
{
    stmfd sp!, {r0, lr}
    add r0, r0, #72
    ldr r1, =0x0206b248
    blx r1
    ldmfd sp!, {r0, lr}
    mrs r1, cpsr
    bic r1, r1, #31
    orr r1, r1, #211
    msr CPSR_c, r1
    ldr r1, [r0], #4
    msr SPSR_fsxc, r1
    ldr sp, [r0, #64]
    ldr lr, [r0, #60]
    ldmia r0, {r0-r12, sp, lr}^
    mov r0, r0
    subs pc, lr, #4
}
