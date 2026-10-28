struct Field {
    unsigned char before[66];
    unsigned short value;
};

int func_02013144(struct Field *object)
{
    return object->value;
}
