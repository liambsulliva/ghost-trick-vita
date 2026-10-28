struct Field {
    unsigned char before[80];
    unsigned char value;
};

void func_02032900(struct Field *object, int value)
{
    object->value = value;
}
