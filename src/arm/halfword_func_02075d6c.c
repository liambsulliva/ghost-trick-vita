extern char data_020d1280;
extern int func_020760c8();
extern int func_020761d0();
extern unsigned char func_0207621c();

void func_02075d6c(void)

{
    unsigned char bVar1;

    if (*((unsigned short *)&data_020d1280) != 0) {
        return;
    }
    *((unsigned short *)&data_020d1280) = 1;
    func_020760c8();
    do {
        bVar1 = func_0207621c(0xc,1);
    } while (!bVar1);
    func_020761d0(0xc,((int)0x02073690u));
    return;
}
