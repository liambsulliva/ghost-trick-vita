void func_02052340(unsigned char *p)
{
    int value;
    if (*(int *)(p + 0x28) > 0) {
        value = *(int *)(p + 0x28) - *(int *)(p + 0x2c);
        *(int *)(p + 0x28) = value;
        if (value < 0) {
            *(int *)(p + 0x28) = 0;
        }
    }
}
