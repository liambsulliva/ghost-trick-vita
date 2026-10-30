asm long long func_0207b674(long long a, long long b)
{
    stmfd sp!, {r4, r5, lr}
    umull r5, r4, r0, r2
    mla r4, r0, r3, r4
    mla r4, r2, r1, r4
    mov r1, r4
    mov r0, r5
    ldmfd sp!, {r4, r5, lr}
    bx lr
}
