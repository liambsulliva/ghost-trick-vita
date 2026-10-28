struct Field {
    unsigned char before[58];
    unsigned char value;
};

void func_02029780(struct Field *object, int value)
{
    object->value = value;
}
