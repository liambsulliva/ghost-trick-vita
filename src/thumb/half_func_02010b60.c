struct Field {
    unsigned char before[4];
    unsigned short value;
};

int func_02010b60(struct Field *object)
{
    return object->value;
}
