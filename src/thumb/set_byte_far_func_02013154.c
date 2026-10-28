struct Field {
    unsigned char before[138];
    unsigned char value;
};

void func_02013154(struct Field *object, int value)
{
    object->value = value;
}
