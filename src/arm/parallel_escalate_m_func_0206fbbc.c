int func_0206fbbc(int *level)
{
    if (*(volatile unsigned int *)0x04000600 & 0x4000) {
        return -1;
    } else {
        *level = (int)((*(volatile unsigned int *)0x04000600 & 0x1f00) >> 8);
        return 0;
    }
}
