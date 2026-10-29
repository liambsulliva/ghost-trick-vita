struct Field {
    unsigned char before[126];
    unsigned short value;
};

int func_02062e40(struct Field *object)
{
    return object->value;
}
