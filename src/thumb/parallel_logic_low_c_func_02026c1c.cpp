struct VTarget {
    virtual void slot0();
    virtual void slot1();
    virtual void slot2();
    virtual void slot3(int a, int b);
};

extern "C" {
extern signed char data_020cee88;
void func_0202fbcc(unsigned char *buf, int n);
int func_0200eac4(unsigned char *buf);
unsigned int func_0200eab4(unsigned char *buf);
unsigned int func_0200eab8(unsigned char *buf);
void func_0200eabc(unsigned char *buf, int n);
int func_02008828(unsigned int p, unsigned int q);
int func_02026c10(unsigned int p);
unsigned int func_0200daac(unsigned int p);
int func_02078fbc(unsigned int p, signed char *q);
unsigned int func_020082f8(unsigned int p, unsigned int q);
unsigned int func_02009900();
unsigned int func_02026d04();
unsigned int func_0205176c(signed char *p);
}

extern "C" void func_02026c1c(VTarget *param_1, unsigned int param_2, signed char *param_3, unsigned int param_4)
{
    signed char cVar1;
    unsigned int uVar2;
    unsigned int uVar3;
    int iVar4;
    unsigned int uVar5;
    unsigned char auStack_28[16];
    unsigned int uStack_18;

    uStack_18 = param_4;
    func_0202fbcc(auStack_28, 5);
    cVar1 = *param_3;
    if (cVar1 == '*') {
        uVar5 = (unsigned int)&data_020cee88;
        while (iVar4 = func_0200eac4(auStack_28), iVar4 != 0) {
            uVar2 = func_0200eab4(auStack_28);
            iVar4 = func_02008828(uVar2, uVar5);
            if (iVar4 != 0) {
                uVar2 = func_0200eab8(auStack_28);
                uVar3 = (unsigned int)func_02026c10(param_2);
                param_1->slot3((int)uVar2, (int)uVar3);
            }
            func_0200eabc(auStack_28, 0);
        }
    }
    else if (cVar1 == '@') {
        while (iVar4 = func_0200eac4(auStack_28), iVar4 != 0) {
            uVar5 = func_0200eab4(auStack_28);
            iVar4 = func_02008828(uVar5, (unsigned int)&data_020cee88);
            if (iVar4 != 0) {
                uVar5 = func_0200eab8(auStack_28);
                uVar2 = func_0200daac(uVar5);
                iVar4 = func_02078fbc(uVar2, param_3 + 1);
                if (iVar4 != 0) {
                    uVar2 = (unsigned int)func_02026c10(param_2);
                    param_1->slot3((int)uVar5, (int)uVar2);
                }
            }
            func_0200eabc(auStack_28, 0);
        }
    }
    else {
        if (cVar1 == '?') {
            func_02009900();
            uVar5 = func_02026d04();
            uVar5 = func_020082f8(uVar5, (unsigned int)&data_020cee88);
        }
        else {
            uVar5 = func_0205176c(param_3);
        }
        if (uVar5 != 0) {
            iVar4 = func_02026c10(param_2);
            param_1->slot3((int)uVar5, iVar4);
        }
    }
}
