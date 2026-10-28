struct Field {
    unsigned char before[44];
    int value;
};

void func_02020e50(struct Field *object, int value)
{
    object->value = value;
}
