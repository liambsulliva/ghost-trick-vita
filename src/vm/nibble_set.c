struct NibbleField {
    unsigned char before[0x1f];
    unsigned char flags;
};

void func_020280b4(struct NibbleField *object, int value)
{
    object->flags = (object->flags & ~0xf) | (value & 0xf);
}
