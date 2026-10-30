typedef struct Key {
    char pad[0x14];
    void *data;
    unsigned int size;
} Key;

extern Key data_020d0dc0;
extern void func_02072fa8(void *dest, unsigned int data, unsigned int size);
extern void func_0207303c(const void *src, void *dst, unsigned int size);
extern int func_02071fac(int value);
extern void func_020722c8(void *digest, const void *bin, unsigned int binLen, const void *key, unsigned int keyLen);

int func_0206cacc(unsigned int *expect, const void *bin, unsigned int binLen, int lock)
{
    unsigned int digest[5];
    unsigned int key[16];
    int old;
    unsigned int i;

    old = 0;
    func_02072fa8(digest, 0, 20);
    func_0207303c(data_020d0dc0.data, key, data_020d0dc0.size);
    if (lock != 0) {
        old = func_02071fac(1);
    }
    func_020722c8(digest, bin, binLen, key, data_020d0dc0.size);
    if (lock != 0) {
        func_02071fac(old);
    }
    for (i = 0; i < 20; i += 4) {
        if (*(unsigned int *)((char *)digest + i) != *(unsigned int *)((char *)expect + i)) {
            break;
        }
    }
    return i == 20;
}
