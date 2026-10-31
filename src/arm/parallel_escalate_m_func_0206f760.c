extern void func_02071e74(int value);

void func_0206f760(int value)
{
    *(volatile unsigned int *)0x04000400 = 22;
    func_02071e74(value);
}
