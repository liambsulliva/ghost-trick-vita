struct Field {
    unsigned char before[4];
    unsigned short value;
};

void func_02037ac4(struct Field *object, int value)
{
    object->value = value;
}
