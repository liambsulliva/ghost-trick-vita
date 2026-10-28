struct Field {
    unsigned char before[198];
    unsigned char value;
};

void func_02010c20(struct Field *object, int value)
{
    object->value = value;
}
