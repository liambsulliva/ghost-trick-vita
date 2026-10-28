struct FlagByte12 {
    unsigned char before[0x12];
    unsigned char bit0 : 1;
    unsigned char bit1 : 1;
    unsigned char bit2 : 1;
    unsigned char rest : 5;
};

void func_02008560(struct FlagByte12 *object)
{
    if (!object->bit2) {
        object->bit0 = 1;
        object->bit1 = 0;
    }
}
