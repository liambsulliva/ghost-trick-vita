extern void *data_020d30c8;
extern char data_020d30cc[];
extern char data_020d34f0[];
extern unsigned int func_02075c98(void);
extern unsigned int func_02075cac(unsigned int bits);
extern void func_0207a250(void *queue);
extern void func_0207a264(void *arg);
extern void func_02073ed4(void *thread, void (*func)(void *), void *arg, void *stack, unsigned int size, unsigned int prio);
extern void func_02074188(void *thread);

void func_0207a1c4(unsigned int *thread)
{
    unsigned int bak;

    bak = func_02075c98();
    if (data_020d30c8 == 0) {
        data_020d30c8 = thread;
        func_0207a250(thread + 49);
        func_0207a250(data_020d30cc);
        thread[48] = 0;
        func_02073ed4(thread, func_0207a264, thread, data_020d34f0, 0x400, 20);
        func_02074188(thread);
    }
    func_02075cac(bak);
}
