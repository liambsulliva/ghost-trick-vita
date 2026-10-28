struct Field {
    unsigned char before[52];
    unsigned char value;
};

void func_020454ec(struct Field *object, int value)
{
    object->value = value;
}
