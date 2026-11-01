asm void func_02071e74(register const void *src, register void *dest)
{
    stmfd sp!, {r4-r8}
    ldmia r0!, {r2-r8, r12}
    stmia r1, {r2-r8, r12}
    ldmia r0!, {r2-r8, r12}
    stmia r1, {r2-r8, r12}
    ldmfd sp!, {r4-r8}
    bx lr
}
