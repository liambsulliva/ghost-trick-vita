struct Field {
    unsigned char before[20];
    int value;
};

int func_02010820(struct Field *object)
{
    return object->value;
}
