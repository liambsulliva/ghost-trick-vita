typedef struct PMWork02078a74 {
    char pad0[4];
    unsigned int offCount;
    unsigned int onCount;
    char padc[8];
    unsigned int lcdFlag;
} PMWork02078a74;

extern PMWork02078a74 data_020d2ec0;

extern void func_02075d14(unsigned int cycles);
extern int func_020784ac(int led);
extern int func_02078448(int led, void *callback, void *arg);
extern int func_02078638(unsigned int on);
extern void func_020780a4(void);

int func_02078a74(int sw, int led, int skip, int isSync)
{
    switch (sw) {
    case 1:
        if (!skip && *(unsigned int *)0x02fffc3c - data_020d2ec0.offCount <= 7) {
            return 0;
        }
        if (led != 0) {
            if (isSync) {
                while (func_020784ac(led) != 0) {
                    func_02075d14(0x51d23);
                }
            } else {
                while (func_02078448(led, 0, 0) != 0) {
                    func_02075d14(0x51d23);
                }
            }
        }
        *(volatile unsigned short *)0x04000304 |= 1;
        while (func_02078638(data_020d2ec0.lcdFlag) != 0) {
            func_02075d14(0x51d23);
        }
        break;
    case 0:
        while (func_02078638(0) != 0) {
            func_02075d14(0x51d23);
        }
        if (*(unsigned int *)0x02fffc3c - data_020d2ec0.onCount <= 2) {
            func_020780a4();
            func_020780a4();
        }
        *(volatile unsigned short *)0x04000304 &= ~1;
        data_020d2ec0.offCount = *(unsigned int *)0x02fffc3c;
        if (led != 0) {
            if (isSync) {
                while (func_020784ac(led) != 0) {
                    func_02075d14(0x51d23);
                }
            } else {
                while (func_02078448(led, 0, 0) != 0) {
                    func_02075d14(0x51d23);
                }
            }
        }
        break;
    }
    return 1;
}
