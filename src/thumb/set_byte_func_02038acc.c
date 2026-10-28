struct Field {
    unsigned char before[9];
    unsigned char value;
};

void func_02038acc(struct Field *object, int value)
{
    object->value = value;
}
