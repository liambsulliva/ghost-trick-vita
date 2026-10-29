extern void func_020108c0(int object);

void func_020106b8(int object,unsigned int value)
{
    unsigned int word;
    unsigned int bit;

    word = *(unsigned int *)(object + 0xc);
    word = word & 0xffff7fff;
    bit = (value << 31) >> 16;
    *(unsigned int *)(object + 0xc) = word | bit;
    if (value != 0) {
        func_020108c0(object);
    }
}
