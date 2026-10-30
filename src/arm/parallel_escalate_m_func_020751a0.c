extern void func_020751b4(void *arg);
extern void func_02075234(void);

asm void func_020751a0(register void *arg)
{
    stmfd sp!, {r0, lr}
    bl func_020751b4
    bl func_02075234
    ldmfd sp!, {r0, lr}
    bx lr
}
