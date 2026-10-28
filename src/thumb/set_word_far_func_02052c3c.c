struct Field {
    unsigned char before[176];
    int value;
};

void func_02052c3c(struct Field *object, int value)
{
    object->value = value;
}
