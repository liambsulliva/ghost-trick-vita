struct Field {
    unsigned char before[132];
    int value;
};

void func_02013164(struct Field *object, int value)
{
    object->value = value;
}
