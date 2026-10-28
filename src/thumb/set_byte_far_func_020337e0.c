struct Field {
    unsigned char before[53];
    unsigned char value;
};

void func_020337e0(struct Field *object, int value)
{
    object->value = value;
}
