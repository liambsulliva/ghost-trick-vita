struct Field {
    unsigned char before[40];
    int value;
};

void func_020183d0(struct Field *object, int value)
{
    object->value = value;
}
