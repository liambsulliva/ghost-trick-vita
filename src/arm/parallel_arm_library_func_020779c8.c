extern unsigned char data_020d2e84[];
void func_020760c8(void);
int func_0207621c(unsigned int, int);
void func_020761d0(unsigned int, int);

void func_020779c8(void)
{
    unsigned char *base;
    int done;

    base = data_020d2e84;
    if (*(unsigned short *)base != 0) {
        return;
    }
    *(unsigned short *)base = 1;
    func_020760c8();
    *(unsigned short *)(base + 16) = 0;
    *(unsigned int *)(base + 4) = 0;
    *(unsigned int *)(base + 20) = 0;
    *(unsigned short *)(base + 54) = 0;
    *(unsigned short *)(base + 52) = 0;
    *(unsigned short *)(base + 58) = 0;
    *(unsigned short *)(base + 56) = 0;
    do {
        done = func_0207621c(6, 1);
    } while (done == 0);
    func_020761d0(6, 0x02077754);
}
