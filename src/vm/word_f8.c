struct WordF8 {
    unsigned char before[0xf8];
    int value;
};

int func_02027b08(struct WordF8 *object)
{
    return object->value;
}
