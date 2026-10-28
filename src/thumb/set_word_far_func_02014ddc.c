struct Field {
    unsigned char before[176];
    int value;
};

void func_02014ddc(struct Field *object, int value)
{
    object->value = value;
}
