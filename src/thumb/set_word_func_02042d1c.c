struct Field {
    unsigned char before[60];
    int value;
};

void func_02042d1c(struct Field *object, int value)
{
    object->value = value;
}
