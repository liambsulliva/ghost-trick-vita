struct Field {
    unsigned char before[28];
    int value;
};

void func_02029018(struct Field *object, int value)
{
    object->value = value;
}
