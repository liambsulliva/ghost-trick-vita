void func_02026608(char *object,unsigned int value)
{
    unsigned int word;

    word = *(unsigned int *)(object + 16);
    *(unsigned int *)(object + 16) = (word & 0xffff00ff) | ((value & 0xff) << 8);
}
