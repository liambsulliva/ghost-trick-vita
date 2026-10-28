struct Field {
    unsigned char before[200];
    unsigned char value;
};

void func_02014228(struct Field *object, int value)
{
    object->value = value;
}
