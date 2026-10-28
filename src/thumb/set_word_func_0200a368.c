struct Field {
    unsigned char before[12];
    int value;
};

void func_0200a368(struct Field *object, int value)
{
    object->value = value;
}
