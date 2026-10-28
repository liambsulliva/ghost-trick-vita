struct Field {
    unsigned char before[44];
    unsigned short value;
};

int func_02026920(struct Field *object)
{
    return object->value;
}
