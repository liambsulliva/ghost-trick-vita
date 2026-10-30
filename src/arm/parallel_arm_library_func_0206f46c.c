
void func_0206f46c(int v)
{
    unsigned short *reg;
    unsigned short old;

    reg = (unsigned short *)0x04000004;
    old = *reg;
    *reg = (old & 0x3f) | ((unsigned int)(v << 24) >> 16) | ((v & 0x100) >> 1);
}
