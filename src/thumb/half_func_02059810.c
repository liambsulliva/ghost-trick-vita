struct Field {
    unsigned char before[16];
    unsigned short value;
};

int func_02059810(struct Field *object)
{
    return object->value;
}
