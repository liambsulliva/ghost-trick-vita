unsigned int func_02023ebc(unsigned char *object, unsigned int key)
{
    unsigned int *slot;
    unsigned int *entry;
    unsigned int i;
    unsigned int wanted;

    if (key == 0) {
        return 0;
    }
    wanted = key;
    slot = (unsigned int *)(object + 0x24);
    wanted &= 0xfffffc00;
    for (i = 1; i < 0xa0; i++, slot++) {
        entry = (unsigned int *)*slot;
        if (entry != 0) {
            if ((entry[4] & 0xfffffc00) == wanted) {
                return (unsigned int)entry;
            }
        }
    }
    return 0;
}
