static inline unsigned int get_vblank_count(void)
{
    return *(volatile unsigned int *)0x02fffc3c;
}

void func_020780a4(void)
{
    volatile unsigned int vcount = get_vblank_count();

    while (vcount == get_vblank_count()) {
    }
}
