struct Field {
    unsigned char before[25];
    unsigned char value;
};

void func_02019318(struct Field *object, int value)
{
    object->value = value;
}
