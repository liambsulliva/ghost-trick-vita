struct Byte11 {
    unsigned char before[0x11];
    unsigned char value;
};

unsigned char func_02027eb4(struct Byte11 *object)
{
    return object->value;
}
