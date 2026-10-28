struct Field {
    unsigned char before[72];
    int value;
};

void func_020516b8(struct Field *object, int value)
{
    object->value = value;
}
