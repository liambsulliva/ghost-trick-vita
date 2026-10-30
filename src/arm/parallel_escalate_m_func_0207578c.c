extern void func_0207579c(void);

asm void func_0207578c(register void *arg)
{
    stmfd sp!, {r0, lr}
    bl func_0207579c
    ldmfd sp!, {r0, lr}
    bx lr
}
