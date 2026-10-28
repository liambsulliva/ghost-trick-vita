struct Field {
    unsigned char before[24];
    unsigned char value;
};

int func_020317b0(struct Field *object)
{
    return object->value;
}
