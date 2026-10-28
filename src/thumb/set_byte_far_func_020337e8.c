struct Field {
    unsigned char before[50];
    unsigned char value;
};

void func_020337e8(struct Field *object, int value)
{
    object->value = value;
}
