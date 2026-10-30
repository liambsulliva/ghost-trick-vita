typedef struct {
    unsigned char on : 1;
    unsigned char loaded : 1;
} Bits0202a940;

typedef struct {
    Bits0202a940 bits;
    unsigned char pad;
    unsigned short id;
    int value;
    int handle;
} Rec0202a940;

void func_0202aa04(char *object, int index, int value);

void func_0202a940(char *object, int index, int value)
{
    int count;

    if (*(unsigned char *)(object + 0x24) != 1 && index != 0xffff) {
        if (((Rec0202a940 *)(object + 0x48))[index].bits.loaded == 0) {
            func_0202aa04(object, index, value);
        }
        count = *(int *)(object + 0x34);
        *(int *)(object + 0x34) = count + 1;
        ((Rec0202a940 *)(object + 0x48))[index].value = count;
    }
}
