asm void func_0207b62c(void)
{
    stmfd sp!, {r4, r5, r6, r7, r11, r12, lr}
    mov r4, #0
    b @L207b640
    stmfd sp!, {r4, r5, r6, r7, r11, r12, lr}
    mov r4, #1
@L207b640:
    orrs r5, r3, r2
    bne @L207b650
    ldmfd sp!, {r4, r5, r6, r7, r11, r12, lr}
    bx lr
@L207b650:
    orrs r5, r1, r3
    dcd 0x1affffa5
    mov r1, r2
    dcd 0xeb000091
    cmp r4, #0
    movne r0, r1
    mov r1, #0
    ldmfd sp!, {r4, r5, r6, r7, r11, r12, lr}
    bx lr
}
