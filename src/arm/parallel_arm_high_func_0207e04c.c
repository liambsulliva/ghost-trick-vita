extern void func_0207d860(void);

asm void func_0207e04c(void)
{
    mov r12, sp
    sub sp, sp, #112
    str r4, [sp, #44]
    str r5, [sp, #48]
    str r6, [sp, #52]
    str r7, [sp, #56]
    str r8, [sp, #60]
    str r9, [sp, #64]
    str r10, [sp, #68]
    str r11, [sp, #72]
    str r12, [sp, #20]
    str r12, [sp, #92]
    str lr, [sp, #16]
    mov r12, #0
    str r12, [sp]
    str r12, [sp, #4]
    str r12, [sp, #8]
    mov r0, sp
    b func_0207d860
}
