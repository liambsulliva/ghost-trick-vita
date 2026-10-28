struct Field {
    unsigned char before[48];
    unsigned short value;
};

void func_02010260(struct Field *object, int value)
{
    object->value = value;
}
