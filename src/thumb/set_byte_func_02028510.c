struct Field {
    unsigned char before[19];
    unsigned char value;
};

void func_02028510(struct Field *object, int value)
{
    object->value = value;
}
