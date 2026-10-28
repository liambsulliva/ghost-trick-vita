struct Field {
    unsigned char before[33];
    unsigned char value;
};

void func_02034250(struct Field *object, int value)
{
    object->value = value;
}
