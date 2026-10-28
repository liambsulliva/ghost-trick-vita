int func_02008364(void *object);

int func_02008350(void *object)
{
    return (func_02008364((char *)object + 0x28) & 1) != 0;
}
