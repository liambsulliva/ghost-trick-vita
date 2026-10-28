struct Field {
    unsigned char before[32];
    unsigned char value;
};

void func_02034248(struct Field *object, int value)
{
    object->value = value;
}
