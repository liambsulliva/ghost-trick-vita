struct Field {
    unsigned char before[60];
    int value;
};

void func_02059648(struct Field *object, int value)
{
    object->value = value;
}
