struct Field {
    unsigned char before[6];
    unsigned short value;
};

int func_02010b64(struct Field *object)
{
    return object->value;
}
