struct Field {
    unsigned char before[124];
    int value;
};

void func_02052c44(struct Field *object, int value)
{
    object->value = value;
}
