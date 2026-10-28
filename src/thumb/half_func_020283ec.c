struct Field {
    unsigned char before[14];
    unsigned short value;
};

int func_020283ec(struct Field *object)
{
    return object->value;
}
