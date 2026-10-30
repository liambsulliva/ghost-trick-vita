
unsigned long long func_020753bc(void);
void func_0207578c(void *arg);
void func_02073550(unsigned int timerNo, void (*callback)(void *), void *arg);
unsigned int func_020735c0(unsigned int v);

void func_0207545c(char *alarm)
{
    long long delta;
    unsigned long long tick;
    unsigned short timerCount;

    tick = func_020753bc();
    timerCount = 0;
    *(volatile unsigned short *)0x04000106 = 0;
    delta = (long long)(*(unsigned long long *)(alarm + 0xc) - tick);
    func_02073550(1, func_0207578c, 0);
    if (delta < 0) {
        timerCount = (unsigned short)~1;
    } else if (delta < 0x10000) {
        timerCount = (unsigned short)(~delta);
    }
    *(volatile unsigned short *)0x04000104 = timerCount;
    *(volatile unsigned short *)0x04000106 = 0xc1;
    func_020735c0(0x10);
}
