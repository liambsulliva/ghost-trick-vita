asm void func_0207dbf4(void)
{
    stmfd sp!, {r4, r5, r6, r7, r11, lr}
    sub sp, sp, #24
    mov r11, sp
    mov r7, r0
    mov r6, r1
    mov r5, r2
    mov r4, r3
    str sp, [r11, #20]
    cmp r6, r7
    addls sp, r11, #24
    ldmlsfd sp!, {r4, r5, r6, r7, r11, pc}
loop:
    sub r6, r6, r5
    mov r0, r6
    blx r4
    cmp r6, r7
    bhi loop
    add sp, r11, #24
    ldmfd sp!, {r4, r5, r6, r7, r11, pc}
}
