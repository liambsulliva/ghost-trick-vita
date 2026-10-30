struct TwoBitField {
    unsigned char before[0x1e];
    unsigned char flags;
};

void func_020394f0(struct TwoBitField *object, int value)
{
    object->flags = (object->flags & ~3) | (value & 3);
}
