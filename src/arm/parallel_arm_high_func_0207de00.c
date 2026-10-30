extern void func_0207da08(void);
extern void func_0207d954(void);
extern void func_0207e04c(void);

asm void func_0207de00(void)
{
    str sp, [r11, #20]
    subs r7, r7, #1
    beq @tail
@loop:
    sub r4, r4, r6
    mov r0, r4
    blx r5
    subs r7, r7, #1
    bne @loop
    b @tail
    bl func_0207da08
    add r0, r11, #0
    bl func_0207d954
    ldr sp, [r11, #20]
@tail:
    bl func_0207e04c
    add r0, r11, #24
    bl func_0207d954
    add sp, r11, #48
    ldmfd sp!, {r4, r5, r6, r7, r11, pc}
}
