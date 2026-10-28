struct Field {
    unsigned char before[64];
    unsigned short value;
};

int func_020130cc(struct Field *object)
{
    return object->value;
}
