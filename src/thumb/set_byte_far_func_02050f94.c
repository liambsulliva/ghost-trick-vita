struct Field {
    unsigned char before[88];
    unsigned char value;
};

void func_02050f94(struct Field *object, int value)
{
    object->value = value;
}
