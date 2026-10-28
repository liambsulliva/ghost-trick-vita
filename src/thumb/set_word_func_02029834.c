struct Field {
    unsigned char before[92];
    int value;
};

void func_02029834(struct Field *object, int value)
{
    object->value = value;
}
