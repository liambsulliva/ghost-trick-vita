struct Field {
    unsigned char before[52];
    int value;
};

void func_020130ec(struct Field *object, int value)
{
    object->value = value;
}
