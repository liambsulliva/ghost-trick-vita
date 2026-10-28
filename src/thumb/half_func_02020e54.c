struct Field {
    unsigned char before[6];
    unsigned short value;
};

int func_02020e54(struct Field *object)
{
    return object->value;
}
