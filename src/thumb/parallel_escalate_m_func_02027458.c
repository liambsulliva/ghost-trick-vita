extern int func_020274ac(unsigned int value);

void func_02027458(unsigned int param_1)
{
    unsigned int index;
    unsigned char *base;

    base = (unsigned char *)func_020274ac(param_1);
    index = param_1 & 0xffff6fff;
    base[index >> 3] |= (unsigned char)(1 << (index & 7));
}
