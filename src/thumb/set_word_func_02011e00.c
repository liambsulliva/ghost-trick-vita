struct Field {
    unsigned char before[4];
    int value;
};

void func_02011e00(struct Field *object, int value)
{
    object->value = value;
}
