struct Field {
    unsigned char before[20];
    int value;
};

void func_02029704(struct Field *object, int value)
{
    object->value = value;
}
