struct Field {
    unsigned char before[4];
    int value;
};

int func_020223b4(struct Field *object)
{
    return object->value;
}
