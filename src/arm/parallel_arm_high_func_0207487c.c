asm void func_0207487c(register unsigned int start, register unsigned int size)
{
    mov r12, #0
    add r1, r1, r0
    bic r0, r0, #31
@loop:
    mcr p15, 0, r12, c7, c10, 4
    mcr p15, 0, r0, c7, c14, 1
    add r0, r0, #32
    cmp r0, r1
    blt @loop
    bx lr
}
