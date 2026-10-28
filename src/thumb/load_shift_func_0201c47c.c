struct Field {
    unsigned char before[52];
    int value;
};

int func_0201c47c(struct Field *object)
{
    return object->value >> 12;
}
