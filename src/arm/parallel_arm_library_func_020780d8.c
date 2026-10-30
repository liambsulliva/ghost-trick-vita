extern unsigned char data_020d2ec0[];
void func_020760c8(void);
int func_0207621c(unsigned int, int);
void func_020761d0(unsigned int, int);
void WaitByLoop(int);

void func_020780d8(void)
{
    unsigned int value;

    if (*(unsigned short *)data_020d2ec0 != 0) {
        return;
    }
    *(unsigned short *)data_020d2ec0 = 1;
    *(unsigned int *)(data_020d2ec0 + 44) = 0;
    *(unsigned int *)(data_020d2ec0 + 48) = 0;
    func_020760c8();
    if (func_0207621c(8, 1) == 0) {
        do {
            WaitByLoop(100);
        } while (func_0207621c(8, 1) == 0);
    }
    func_020761d0(8, 0x0207816c);
    value = *(unsigned int *)0x02fffc3c;
    *(unsigned int *)(data_020d2ec0 + 8) = value;
    *(unsigned int *)(data_020d2ec0 + 4) = value;
}
