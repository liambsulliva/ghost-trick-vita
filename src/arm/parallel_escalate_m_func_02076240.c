typedef union {
    unsigned int raw;
    struct {
        unsigned int tag:5;
        unsigned int err:1;
        unsigned int data:26;
    } e;
} FifoMessage;

extern int func_02075c98(void);
extern void func_02075cac(int state);

int func_02076240(int tag, unsigned int data, int err)
{
    FifoMessage message;
    int state;

    message.e.tag = tag;
    message.e.err = err;
    message.e.data = data;
    if (*(volatile unsigned short *)0x04000184 & 0x4000) {
        *(volatile unsigned short *)0x04000184 |= 0xc000;
        return -1;
    }
    state = func_02075c98();
    if (*(volatile unsigned short *)0x04000184 & 2) {
        func_02075cac(state);
        return -2;
    }
    *(volatile unsigned int *)0x04000188 = message.raw;
    func_02075cac(state);
    return 0;
}
