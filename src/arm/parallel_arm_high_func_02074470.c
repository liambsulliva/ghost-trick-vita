asm int func_02074470(register int *context)
{
    stmfd sp!, {r0, lr}
    add r0, r0, #72
    ldr r1, =0x0206b208
    blx r1
    ldmfd sp!, {r0, lr}
    add r1, r0, #0
    mrs r2, cpsr
    str r2, [r1], #4
    mov r0, #211
    msr CPSR_c, r0
    str sp, [r1, #64]
    msr CPSR_c, r2
    mov r0, #1
    stmia r1, {r0-r12, sp, lr}
    add r0, pc, #8
    str r0, [r1, #60]
    mov r0, #0
    bx lr
}
