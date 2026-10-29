struct Field {
    unsigned char before[32];
    unsigned char value;
};

int func_020654dc(struct Field *object)
{
    return object->value;
}
