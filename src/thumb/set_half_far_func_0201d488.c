struct Field {
    unsigned char before[64];
    unsigned short value;
};

void func_0201d488(struct Field *object, int value)
{
    object->value = value;
}
