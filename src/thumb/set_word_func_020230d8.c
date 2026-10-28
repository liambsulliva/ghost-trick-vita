struct Field {
    unsigned char before[8];
    int value;
};

void func_020230d8(struct Field *object, int value)
{
    object->value = value;
}
