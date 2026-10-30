asm void func_0206b248(register const void *context)
{
    stmfd sp!, {r4}
    ldr r1, =0x04000290
    ldmia r0, {r2, r3, r4, r12}
    stmia r1, {r2, r3, r4, r12}
    ldrh r2, [r0, #24]
    ldrh r3, [r0, #26]
    strh r2, [r1, #-16]
    strh r3, [r1, #32]
    add r0, r0, #16
    add r1, r1, #40
    ldmia r0, {r2, r3}
    stmia r1, {r2, r3}
    ldmfd sp!, {r4}
    bx lr
}
