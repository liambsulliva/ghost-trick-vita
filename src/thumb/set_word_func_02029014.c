struct Field {
    unsigned char before[24];
    int value;
};

void func_02029014(struct Field *object, int value)
{
    object->value = value;
}
