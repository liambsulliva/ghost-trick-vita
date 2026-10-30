void func_02026838(unsigned char *object,unsigned short value)
{
    unsigned char *cell;
    cell = object;
    cell = cell + 0xca;
    *(unsigned short *)cell = value;
    *(unsigned short *)(object + 0xcc) = value;
}
