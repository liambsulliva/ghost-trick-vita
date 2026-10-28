struct Field {
    unsigned char before[164];
    int value;
};

void func_02027888(struct Field *object, int value)
{
    object->value = value;
}
