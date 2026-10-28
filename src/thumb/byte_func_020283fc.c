struct Field {
    unsigned char before[16];
    unsigned char value;
};

int func_020283fc(struct Field *object)
{
    return object->value;
}
