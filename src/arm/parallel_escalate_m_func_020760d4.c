typedef struct {
    char pad[0x388];
    unsigned int pxiHandleChecker[2];
} OSSystemWork;

extern OSSystemWork data_02fffc00;

extern unsigned short data_020d12a4;
extern void *data_020d12a8[32];
extern unsigned int func_02075c98(void);
extern void func_02075cac(unsigned int enabled);
extern void func_02073620(unsigned int mask);
extern void func_02073480(unsigned int mask, void (*handler)());
extern void func_020735c0(unsigned int mask);
extern void func_020762b8();

void func_020760d4(void)
{
    unsigned int enabled;
    int i;
    int timeout;
    int c;

    enabled = func_02075c98();
    if (data_020d12a4 == 0) {
        data_020d12a4 = 1;
        data_02fffc00.pxiHandleChecker[0] = 0;
        for (i = 0; i < 32; i++) {
            data_020d12a8[i] = 0;
        }
        *(volatile unsigned short *)0x04000184 = 0xc408;
        func_02073620(0x40000);
        func_02073480(0x40000, func_020762b8);
        func_020735c0(0x40000);
        for (i = 0; ; i++) {
            c = *(volatile unsigned short *)0x04000180 & 15;
            *(volatile unsigned short *)0x04000180 = (unsigned short)(c << 8);
            if (c == 0 && i > 4) {
                break;
            }
            for (timeout = 1000; (*(volatile unsigned short *)0x04000180 & 15) == c; timeout--) {
                if (timeout <= 0) {
                    i = 0;
                    break;
                }
            }
        }
    }
    func_02075cac(enabled);
}
