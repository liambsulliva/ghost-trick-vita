struct Field {
    unsigned char before[100];
    unsigned char value;
};

void func_020164a8(struct Field *object, int value)
{
    object->value = value;
}
