struct Word24 {
    unsigned int low : 24;
    unsigned int high : 8;
};
unsigned int func_02020be4(unsigned char *object)
{
    struct Word24 *word;
    word = (struct Word24 *)(object + 0x14);
    return word->low;
}
