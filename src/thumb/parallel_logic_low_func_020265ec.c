void func_020265ec(char *object,unsigned int value)
{
    unsigned int word;

    word = *(unsigned int *)(object + 16);
    value = (value << 24) >> 24;
    word = word & 0xffffff00;
    *(unsigned int *)(object + 16) = word | value;
}
