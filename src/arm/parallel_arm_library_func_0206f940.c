extern void func_0206fc14(volatile unsigned int *param_1);

void func_0206f940(void)
{
    volatile unsigned int *p;

    p = (volatile unsigned int *)0x04000400;
    func_0206fc14(p);
    do {
    } while ((p[0x80] & 0x8000000) != 0);
}
