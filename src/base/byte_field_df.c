struct ByteFieldDf {
    unsigned char before[0xdf];
    unsigned char value : 8;
};

void func_020083e0(struct ByteFieldDf *object, int value)
{
    object->value = (unsigned char)value;
}
