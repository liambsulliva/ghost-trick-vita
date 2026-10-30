static inline void set_rom1st(int c)
{
    *(volatile unsigned short *)0x04000204 = (unsigned short)((c << 2) | (*(volatile unsigned short *)0x04000204 & ~0xc));
}

static inline void set_rom2nd(int c)
{
    *(volatile unsigned short *)0x04000204 = (unsigned short)((c << 4) | (*(volatile unsigned short *)0x04000204 & ~0x10));
}

void func_02079cf0(int *cycle)
{
    set_rom1st(cycle[0]);
    set_rom2nd(cycle[1]);
}
