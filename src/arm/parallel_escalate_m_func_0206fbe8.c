int func_0206fbe8(int *level)
{
    if (*(volatile unsigned int *)0x04000600 & 0x4000) {
        return -1;
    } else {
        *level = (int)((*(volatile unsigned int *)0x04000600 & 0x2000) >> 13);
        return 0;
    }
}
