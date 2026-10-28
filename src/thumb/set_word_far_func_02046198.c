struct Field {
    unsigned char before[208];
    int value;
};

void func_02046198(struct Field *object, int value)
{
    object->value = value;
}
