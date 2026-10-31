asm void func_02072f08(register unsigned int data, register void *destp, register unsigned int size)
{
    stmfd sp!, {r4-r9}
    add r9, r1, r2
    mov r12, r2, lsr #5
    add r12, r1, r12, lsl #5
    mov r2, r0
    mov r3, r2
    mov r4, r2
    mov r5, r2
    mov r6, r2
    mov r7, r2
    mov r8, r2
@10:
    cmp r1, r12
    stmltia r1!, {r0, r2-r8}
    blt @10
@20:
    cmp r1, r9
    stmltia r1!, {r0}
    blt @20
    ldmfd sp!, {r4-r9}
    bx lr
}
