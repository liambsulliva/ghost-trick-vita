struct Field {
    unsigned char before[8];
    unsigned short value;
};

void func_0202dfc0(struct Field *object, int value)
{
    object->value = value;
}
