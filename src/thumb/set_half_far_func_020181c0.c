struct Field {
    unsigned char before[162];
    unsigned short value;
};

void func_020181c0(struct Field *object, int value)
{
    object->value = value;
}
