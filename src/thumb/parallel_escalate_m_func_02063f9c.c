extern int func_02063fec();

void func_02063f9c(void)
{
    int base;
    unsigned int x;
    unsigned int y;
    int row;
    int off;
    unsigned short c;
    unsigned char a;

    base = func_02063fec();
    x = 0;
    do {
        y = 0;
        row = base + x * 2;
        do {
            off = y * 0x200;
            c = *(unsigned short *)(row + off);
            y = y + 1;
            a = 1 & ((int)c >> 15);
            *(unsigned short *)(row + off) = (a << 15) | (unsigned short)((unsigned short)(0xffff - c) & 0x7fff);
        } while (y < 0xc0);
        x = x + 1;
    } while (x < 0x100);
}
