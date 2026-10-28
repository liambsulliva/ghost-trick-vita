struct Field {
    unsigned char before[16];
    int value;
};

void func_02011dfc(struct Field *object, int value)
{
    object->value = value;
}
