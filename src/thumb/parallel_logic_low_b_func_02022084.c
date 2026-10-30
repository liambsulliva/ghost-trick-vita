typedef int (*WriteFn)(void *, void *, unsigned int);
typedef unsigned int (*TellFn)(void *);
struct StreamVtbl {
    unsigned int slots[9];
    WriteFn write;
    void *reserved;
    TellFn tell;
};
struct Stream {
    struct StreamVtbl *vtbl;
};
extern int func_02072fa8(void *dst, int value, unsigned int size);

int func_02022084(struct Stream **holder, unsigned int a1, unsigned int a2, unsigned int a3)
{
    unsigned char zeros[8];
    unsigned int pad;

    func_02072fa8(zeros, 0, 8);
    pad = (*holder)->vtbl->tell(*holder) & 7;
    if (pad != 0) {
        pad = 8 - pad;
    }
    if (pad != 0) {
        (*holder)->vtbl->write(*holder, zeros, pad);
    }
    return (*holder)->vtbl->write(*holder, &a1, 8);
}
