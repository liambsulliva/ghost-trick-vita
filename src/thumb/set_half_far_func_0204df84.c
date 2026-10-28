struct Field {
    unsigned char before[188];
    unsigned short value;
};

void func_0204df84(struct Field *object, int value)
{
    object->value = value;
}
