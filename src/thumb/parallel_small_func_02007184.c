extern char data_020b18d8;
extern char data_020b18dc;
extern void func_0200743c(char *object);

void func_02007184(int a, int b, int c, unsigned int volume, unsigned short arg, unsigned char slot)
{
    int *p;

    p = (int *)&data_020b18d8;
    p[1] = 0x10;
    p[2] = (a << 16) | (b & 0xffff);
    p[3] = c << 16;
    p[4] = 0x80000000 | volume;
    p[5] = arg;
    p[6] = slot;
    func_0200743c(&data_020b18dc);
}
