struct Field {
    unsigned char before[126];
    unsigned char value;
};

void func_02060d54(struct Field *object, int value)
{
    object->value = value;
}
