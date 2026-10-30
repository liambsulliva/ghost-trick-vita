typedef struct {
    unsigned char bit0 : 1;
    unsigned char bit1 : 1;
    unsigned char bit2 : 1;
    unsigned char bit3 : 1;
} Bits0202a770;

extern "C" {
extern void func_020072cc(int a, int b);

void func_0202a770(char *object)
{
    Bits0202a770 *bits;

    if (*(unsigned char *)(object + 0x24) == 2) {
        return;
    }
    bits = (Bits0202a770 *)(object + 0x38);
    if (bits->bit1 == 0 || bits->bit2 != 0) {
        return;
    }
    func_020072cc(-1, *(short *)(object + 0x3a));
    bits->bit3 = bits->bit3 == 0;
}
}
