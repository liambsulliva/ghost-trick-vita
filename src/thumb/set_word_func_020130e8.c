struct Field {
    unsigned char before[56];
    int value;
};

void func_020130e8(struct Field *object, int value)
{
    object->value = value;
}
