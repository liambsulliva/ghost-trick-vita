struct Field {
    unsigned char before[126];
    unsigned short value;
};

void func_0204c540(struct Field *object, int value)
{
    object->value = value;
}
