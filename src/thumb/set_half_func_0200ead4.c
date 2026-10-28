struct Field {
    unsigned char before[2];
    unsigned short value;
};

void func_0200ead4(struct Field *object, int value)
{
    object->value = value;
}
