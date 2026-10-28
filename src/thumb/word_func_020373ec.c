struct Field {
    unsigned char before[8];
    int value;
};

int func_020373ec(struct Field *object)
{
    return object->value;
}
