struct Field {
    unsigned char before[84];
    unsigned char value;
};

int func_020118a0(struct Field *object)
{
    return object->value;
}
