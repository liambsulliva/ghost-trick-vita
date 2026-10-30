unsigned int func_020222f4(unsigned int *object,unsigned int value)
{
    unsigned int index;
    unsigned int *words;
    unsigned char *bytes;
    index = object[0];
    words = (unsigned int *)object[1];
    words[index] = value;
    bytes = (unsigned char *)object[2];
    bytes[object[0]] = 0;
    object[0] = object[0] + 1;
    return object[0] - 1;
}
