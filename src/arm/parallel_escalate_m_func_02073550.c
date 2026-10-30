typedef struct {
    void (*func)(void *);
    unsigned int enable;
    void *arg;
} CallbackInfo;

extern CallbackInfo data_020d0ed4[];
unsigned int func_020735c0(unsigned int v);

void func_02073550(unsigned int timerNo, void (*callback)(void *), void *arg)
{
    unsigned int imask;

    imask = 1 << (timerNo + 3);
    data_020d0ed4[timerNo].func = callback;
    data_020d0ed4[timerNo].arg = arg;
    func_020735c0(imask);
    data_020d0ed4[timerNo].enable = 1;
}
