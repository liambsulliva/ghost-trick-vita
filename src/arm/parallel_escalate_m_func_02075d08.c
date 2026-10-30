asm void func_02075d08(unsigned int count)
{
loop:
    subs r0, r0, #4
    bhs loop
    bx lr
}
