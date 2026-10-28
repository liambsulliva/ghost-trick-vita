struct Field {
    unsigned char before[12];
    int value;
};

void func_02038ad0(struct Field *object, int value)
{
    object->value = value;
}
