struct Field {
    unsigned char before[17];
    unsigned char value;
};

void func_02028508(struct Field *object, int value)
{
    object->value = value;
}
