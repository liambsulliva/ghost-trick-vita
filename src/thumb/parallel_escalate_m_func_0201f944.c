typedef struct {
    unsigned char low : 7;
    unsigned char bit7 : 1;
} Bits0201f944;

extern int func_0201f934(int object, unsigned int index);

int func_0201f944(int object, unsigned int index)
{
    unsigned char *slot;

    if (func_0201f934(object, index) == 0) {
        return -1;
    }
    slot = ((unsigned char **)*(int *)(object + 0xc))[index];
    if (slot == 0 || ((Bits0201f944 *)(slot + 6))->bit7 != 0) {
        return -1;
    }
    return slot[4];
}
