struct ByteC2 {
    unsigned char before[0xc2];
    unsigned char value;
};

void func_020083fc(struct ByteC2 *object, unsigned char value)
{
    object->value = value;
}
