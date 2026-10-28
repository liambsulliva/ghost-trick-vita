struct Field {
    unsigned char before[8];
    int value;
};

void func_02011e04(struct Field *object, int value)
{
    object->value = value;
}
