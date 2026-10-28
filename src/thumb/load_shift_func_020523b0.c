struct Field {
    unsigned char before[40];
    int value;
};

int func_020523b0(struct Field *object)
{
    return object->value >> 12;
}
