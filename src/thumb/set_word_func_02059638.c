struct Field {
    unsigned char before[20];
    int value;
};

void func_02059638(struct Field *object, int value)
{
    object->value = value;
}
