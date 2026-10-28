struct Field {
    unsigned char before[56];
    int value;
};

int func_020130d8(struct Field *object)
{
    return object->value;
}
