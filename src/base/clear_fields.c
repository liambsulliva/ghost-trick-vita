void func_02009830(char *object)
{
    *(int *)(object + 4) = 0;
    *(int *)object = 0;
    object[6] = 0;
    object[7] = 0;
    *(unsigned short *)(object + 8) = 0;
    *(unsigned short *)(object + 10) = 0;
    *(int *)(object + 12) = 1 << 12;
}
