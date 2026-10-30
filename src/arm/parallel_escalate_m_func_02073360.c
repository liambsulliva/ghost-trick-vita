typedef struct {
    void (*func)(void *);
    unsigned int enable;
    void *arg;
} CallbackInfo02073360;

extern CallbackInfo02073360 data_020d0ea4[];
extern unsigned short data_020b1750[];
extern char data_027e0000;
extern unsigned int func_020735f0(unsigned int v);

void func_02073360(int index)
{
    unsigned int imask;
    void (*callback)(void *);

    imask = 1u << data_020b1750[index];
    callback = data_020d0ea4[index].func;
    data_020d0ea4[index].func = 0;
    if (callback != 0) {
        callback(data_020d0ea4[index].arg);
    }
    *(unsigned int *)(((int)&data_027e0000) + ((int)0x00003ff8u)) = *(unsigned int *)(((int)&data_027e0000) + ((int)0x00003ff8u)) | imask;
    if (data_020d0ea4[index].enable == 0) {
        func_020735f0(imask);
    }
}
