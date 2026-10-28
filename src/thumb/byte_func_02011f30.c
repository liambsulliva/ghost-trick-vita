struct Field {
    unsigned char before[12];
    unsigned char value;
};

int func_02011f30(struct Field *object)
{
    return object->value;
}
