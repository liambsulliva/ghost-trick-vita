extern unsigned int func_0203188c(int id);
extern int func_0200e8fc(unsigned char *object, int id, int offset);

int func_0200e8bc(unsigned char *object, int id, int offset)
{
    unsigned int index;
    int result;

    if (id == 0) {
        return 0;
    }
    index = func_0203188c(id);
    index = index + offset;
    result = func_0200e8fc(object, id, offset);
    (object + 0xb64)[index >> 3] |= (unsigned char)(1 << (index & 7));
    return result;
}
