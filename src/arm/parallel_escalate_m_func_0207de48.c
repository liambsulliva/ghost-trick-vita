asm void func_0207de48(void *block, unsigned int count, unsigned int size, void *dtor)
{
    stmfd sp!, {r4, r5, r6, r7, r11, lr}
    sub sp, sp, #24
    mov r11, sp
    mov r7, r1
    movs r5, r3
    mov r6, r2
    mlane r4, r7, r6, r0
    strne sp, [r11, #20]
    cmpne r7, #0
    addeq sp, r11, #24
    ldmeqfd sp!, {r4, r5, r6, r7, r11, pc}
@1:
    sub r4, r4, r6
    mov r0, r4
    blx r5
    subs r7, r7, #1
    bne @1
    add sp, r11, #24
    ldmfd sp!, {r4, r5, r6, r7, r11, pc}
}
