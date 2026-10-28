struct Field {
    unsigned char before[192];
    unsigned short value;
};

int func_020131bc(struct Field *object)
{
    return object->value;
}
