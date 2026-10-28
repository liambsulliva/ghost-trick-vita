struct TwoBitField {
    unsigned char before[0x1c];
    unsigned char flags;
};

void func_02027fb4(struct TwoBitField *object, int value)
{
    object->flags = (object->flags & ~3) | (value & 3);
}
