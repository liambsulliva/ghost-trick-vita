struct Field {
    unsigned char before[188];
    int value;
};

void func_020379f0(struct Field *object, int value)
{
    object->value = value;
}
