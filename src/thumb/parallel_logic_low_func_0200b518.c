extern void *func_0200aee8(int size);
extern void *func_0200ae2c(void *object);
extern unsigned char *func_02008860(void);
extern int func_0200b588(unsigned char *object, int index);
extern int func_0200b00c(unsigned char *object, int index);

void func_0200b518(unsigned char *object, int index, int first, int second)
{
    int *slot;
    int *entry;

    slot = (int *)(object + 0x18) + index;
    if (*slot == 0) {
        object[10] = (unsigned char)index;
        entry = (int *)func_0200aee8(40);
        if (entry != 0) {
            entry = (int *)func_0200ae2c(entry);
        }
        if (entry != 0) {
            *slot = (int)entry;
            entry[4] = func_0200b588(func_02008860(), index);
            entry[5] = func_0200b00c(func_02008860(), index);
            ((unsigned char *)entry)[12] = (((unsigned char *)entry)[12] & ~1) | 1;
            entry[6] = first;
            entry[7] = second;
            ((int *)object)[3] = first;
            ((int *)object)[4] = second;
        } else {
            *slot = 0;
        }
    }
}
