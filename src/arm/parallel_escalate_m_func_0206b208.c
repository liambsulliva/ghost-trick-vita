asm void func_0206b208(register void *context)
{
    ldr r1, =0x04000290
    stmfd sp!, {r4}
    ldmia r1, {r2, r3, r4, r12}
    stmia r0!, {r2, r3, r4, r12}
    ldrh r12, [r1, #-16]
    add r1, r1, #40
    ldmia r1, {r2, r3}
    stmia r0!, {r2, r3}
    and r12, r12, #3
    ldrh r2, [r1, #-8]
    strh r12, [r0]
    and r2, r2, #1
    strh r2, [r0, #2]
    ldmfd sp!, {r4}
    bx lr
}
