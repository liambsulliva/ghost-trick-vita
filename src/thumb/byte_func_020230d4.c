struct Field {
    unsigned char before[12];
    unsigned char value;
};

int func_020230d4(struct Field *object)
{
    return object->value;
}
