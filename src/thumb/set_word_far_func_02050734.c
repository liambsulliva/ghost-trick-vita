struct Field {
    unsigned char before[188];
    int value;
};

void func_02050734(struct Field *object, int value)
{
    object->value = value;
}
