struct Field {
    unsigned char before[4];
    unsigned short value;
};

void func_0201b588(struct Field *object, int value)
{
    object->value = value;
}
