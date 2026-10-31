extern int func_02005648(int value);
extern int func_020772f8(int value);
extern void func_02007a5c(int mask, unsigned char low, int high);

void func_020055fc(unsigned char *object, int volume)
{
    unsigned int i;
    int result;

    for (i = 0; i < 2; i++) {
        if (*(object + i + 0xf6) != -1) {
            result = func_020772f8(func_02005648(volume * *(object + i + 0xf8) / 0x7f));
            func_02007a5c(1 << *(object + i + 0xf6), result, result >> 8);
        }
    }
}
