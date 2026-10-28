struct Field {
    unsigned char before[4];
    unsigned short value;
};

int func_02020254(struct Field *object)
{
    return object->value;
}
