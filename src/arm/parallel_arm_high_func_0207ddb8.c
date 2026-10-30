asm void func_0207ddb8(void)
{
    stmfd sp!, {r4, r5, r6, r7, r11, lr}
    sub sp, sp, #48
    mov r11, sp
    mov r7, r1
    movs r5, r3
    mov r6, r2
    mlane r4, r7, r6, r0
    strne sp, [r11, #44]
    cmpne r7, #0
    addeq sp, r11, #48
    ldmeqfd sp!, {r4, r5, r6, r7, r11, pc}
@loop:
    sub r4, r4, r6
    mov r0, r4
    blx r5
    subs r7, r7, #1
    bne @loop
    add sp, r11, #48
    ldmfd sp!, {r4, r5, r6, r7, r11, pc}
}
