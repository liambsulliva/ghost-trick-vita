extern unsigned int func_0203188c(int id);

int func_0200e8fc(unsigned char *object, int id, int offset)
{
    unsigned int index;
    if (id == 0) {
        return 0;
    }
    index = func_0203188c(id);
    index = index + offset;
    return ((1 << (index & 7)) & (object + 0xb64)[index >> 3]) != 0;
}
