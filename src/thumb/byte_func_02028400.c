struct Field {
    unsigned char before[17];
    unsigned char value;
};

int func_02028400(struct Field *object)
{
    return object->value;
}
