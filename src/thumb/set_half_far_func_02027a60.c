struct Field {
    unsigned char before[150];
    unsigned short value;
};

void func_02027a60(struct Field *object, int value)
{
    object->value = value;
}
