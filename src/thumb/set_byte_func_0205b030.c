struct Field {
    unsigned char before[12];
    unsigned char value;
};

void func_0205b030(struct Field *object, int value)
{
    object->value = value;
}
