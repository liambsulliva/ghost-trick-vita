struct Field {
    unsigned char before[60];
    unsigned char value;
};

void func_02016fbc(struct Field *object, int value)
{
    object->value = value;
}
