struct Field {
    unsigned char before[16];
    unsigned char value;
};

void func_02028504(struct Field *object, int value)
{
    object->value = value;
}
