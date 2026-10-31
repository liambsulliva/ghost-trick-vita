class Table
{
public:
    virtual int s0();
    virtual int s1();
    virtual int s2();
    virtual unsigned int s3(int a, int b, unsigned int c, unsigned int d);
    virtual int s4();
    virtual int s5();
    virtual int s6();
    virtual int s7(unsigned int a);
    int pad[14];
    int slots;
};

extern "C" {
unsigned char *func_0200883c(void);
unsigned char *func_02008824(unsigned char *p);
int func_0200cb5c(unsigned char *p);
void func_0200cb60(unsigned char *p);
unsigned int func_0200cc34(Table *p, unsigned int a);
}

extern "C" unsigned int func_0200cb04(Table *param_1, unsigned int param_2, unsigned int param_3)
{
    unsigned int uVar1;
    int iVar2;
    int iVar4;
    unsigned char *puVar3;
    uVar1 = func_0200cc34(param_1, param_2);
    if (uVar1 == 0xffffffff) {
        iVar2 = param_1->s7(param_2);
        if (iVar2 != 0) {
            puVar3 = func_0200883c();
            puVar3 = func_02008824(puVar3);
            iVar4 = func_0200cb5c(puVar3);
            uVar1 = param_1->s3(iVar2, iVar4, param_2, param_3);
        }
    } else {
        func_0200cb60((unsigned char *)(param_1->slots + uVar1 * 0x18));
    }
    return uVar1;
}
