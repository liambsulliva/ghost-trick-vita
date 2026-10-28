struct Field {
    unsigned char before[16];
    unsigned char value;
};

void func_0200b760(struct Field *object, int value)
{
    object->value = value;
}
