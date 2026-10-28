struct Field {
    unsigned char before[197];
    unsigned char value;
};

void func_02010c18(struct Field *object, int value)
{
    object->value = value;
}
