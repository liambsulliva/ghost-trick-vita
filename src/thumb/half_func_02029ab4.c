struct Field {
    unsigned char before[12];
    unsigned short value;
};

int func_02029ab4(struct Field *object)
{
    return object->value;
}
