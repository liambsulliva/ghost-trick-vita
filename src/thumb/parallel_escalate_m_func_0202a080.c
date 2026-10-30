typedef struct {
    unsigned char bit0 : 1;
    unsigned char bit1 : 1;
} Bits0202a080;

extern int func_02012100(char *object, int value);
extern void func_02007144(int a, int b, unsigned int c, int d);

void func_0202a080(char *object, int scale, int last)
{
    unsigned int volume;

    if (*(unsigned char *)(object + 0x24) != 2) {
        volume = (int)(func_02012100(object, 0) * scale) >> 8;
        if (volume > 0x7f) {
            volume = 0x7f;
        }
        if (((Bits0202a080 *)(object + 0x38))->bit1 != 0) {
            func_02007144(-1, *(short *)(object + 0x3a), volume, last);
        }
    }
}
