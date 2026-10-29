extern int func_0202239c(int a,char *b);

unsigned int func_02027c08(char *object)
{
    unsigned int value;

    value = func_0202239c(0x020ca00c,object);
    if ((int)value < 0) {
        value = 0;
    }
    else {
        value = value & 0xffff;
    }
    return value & 0xffff;
}
