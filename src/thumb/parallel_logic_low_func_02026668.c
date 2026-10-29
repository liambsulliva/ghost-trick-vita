struct Small {
    unsigned char before[0x1c];
    unsigned int count;
    unsigned int values[4];
};

int func_02026668(struct Small *object,unsigned int value)
{
    unsigned int count;

    count = object->count;
    if (count < 4) {
        object->count = count + 1;
        object->values[count] = value;
        return 1;
    }
    return 0;
}
