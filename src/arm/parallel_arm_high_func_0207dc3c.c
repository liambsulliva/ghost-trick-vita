extern void func_0207da08(void);
extern void func_0207d954(void);

asm void func_0207dc3c(void)
{
    bl func_0207da08
    add r0, r11, #0
    bl func_0207d954
    add sp, r11, #24
    ldmfd sp!, {r4, r5, r6, r7, r11, pc}
}
