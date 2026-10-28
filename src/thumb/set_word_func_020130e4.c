struct Field {
    unsigned char before[60];
    int value;
};

void func_020130e4(struct Field *object, int value)
{
    object->value = value;
}
