struct Field {
    unsigned char before[212];
    int value;
};

void func_02052f14(struct Field *object, int value)
{
    object->value = value;
}
