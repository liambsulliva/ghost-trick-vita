extern unsigned int func_0206f004(void);

unsigned int func_0206ef48(int value)
{
    if (value <= 0) {
        return 0;
    }
    *(volatile unsigned short *)0x040002b0 = 1;
    *(volatile unsigned long long *)0x040002b8 = (unsigned long long)value << 32;
    return func_0206f004();
}
