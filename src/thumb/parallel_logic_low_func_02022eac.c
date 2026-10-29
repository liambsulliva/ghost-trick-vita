extern int func_0206cd7c(int unused,unsigned int value);

int func_02022eac(int object,unsigned int value,unsigned int flag)
{
    int result;

    result = func_0206cd7c(0,value);
    *(unsigned int *)(object + 0x18) = value;
    if (result != 0) {
        *(unsigned char *)(object + 0x10) = 1;
        *(unsigned char *)(object + 0xf) = flag;
        return 1;
    }
    *(unsigned char *)(object + 0x10) = 0;
    *(unsigned char *)(object + 0xf) = 0;
    return 0;
}
