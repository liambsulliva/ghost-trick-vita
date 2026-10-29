struct Field {
    unsigned char before[154];
    unsigned char value;
};

void func_02062e48(struct Field *object, int value)
{
    object->value = value;
}
