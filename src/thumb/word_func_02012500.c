struct Field {
    unsigned char before[4];
    int value;
};

int func_02012500(struct Field *object)
{
    return object->value;
}
