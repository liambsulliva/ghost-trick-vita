typedef struct {
    void (*func)(void *);
    unsigned int enable;
    void *arg;
} CallbackInfo;

extern CallbackInfo data_020d0ea4[];
unsigned int func_020735c0(unsigned int v);

void func_02073508(unsigned int dmaNo, void (*callback)(void *), void *arg)
{
    unsigned int imask;

    imask = 1 << (dmaNo + 8);
    data_020d0ea4[dmaNo].func = callback;
    data_020d0ea4[dmaNo].arg = arg;
    data_020d0ea4[dmaNo].enable = func_020735c0(imask) & imask;
}
