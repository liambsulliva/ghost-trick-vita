struct Field {
    unsigned char before[15];
    unsigned char value;
};

void func_02028514(struct Field *object, int value)
{
    object->value = value;
}
