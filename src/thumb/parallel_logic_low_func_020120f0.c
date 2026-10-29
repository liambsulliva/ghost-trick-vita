struct Record {
    unsigned char before[0x38];
    unsigned char bit0 : 1;
    unsigned char bit1 : 1;
    unsigned char rest : 6;
};

unsigned int func_020120f0(int object,int index)
{
    return ((struct Record *)(object + index * 0x10))->bit1;
}
