struct Small {
    unsigned char before[0x1c];
    unsigned int count;
    unsigned int values[4];
};

int func_02026680(struct Small *object)
{
    unsigned int count;

    count = object->count;
    if (count != 0) {
        count = count - 1;
        object->count = count;
        return object->values[count];
    }
    return -1;
}
