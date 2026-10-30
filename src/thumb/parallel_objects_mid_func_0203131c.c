extern char data_020cff60;

unsigned long long func_02073480(int param_1, unsigned int param_2);
void func_020748f4(void);
void func_020752d4(void);
void func_02073d88(void);
void func_0206f288(void);
void func_020313e4(unsigned char *param_1, int param_2);
int func_02031400(void);
void func_02075e44(unsigned char *param_1);
void func_0206f324(void);
void func_0206f4dc(void);
void func_0203140c(void);
void func_02031420(unsigned char *param_1);
void func_02031430(unsigned char *param_1);
void func_0206f544(int param_1);
void func_02030378(void);
void func_0201f038(void);
void func_02031440(void);
void func_02031450(void);
void func_02031460(void);
void func_0206f494(int param_1, unsigned int param_2);
void func_020735c0(int param_1);
void func_0202286c(void);

void func_0203131c(void)
{
    unsigned char *puVar1;
    unsigned char uVar2;
    int iVar3;
    unsigned char abStack_60[84];

    func_020748f4();
    func_020752d4();
    func_02073d88();
    func_0206f288();
    puVar1 = (unsigned char *)&data_020cff60;
    func_020313e4(puVar1, 0x1c);
    iVar3 = func_02031400();
    *(int *)(puVar1 + 0x10) = iVar3;
    if (puVar1[0x10] == 0) {
        puVar1[0xd] = 1;
    }
    func_02075e44(abStack_60);
    switch (abStack_60[0]) {
    default:
        uVar2 = 1;
        break;
    case 2:
        uVar2 = 2;
        break;
    case 3:
        uVar2 = 3;
        break;
    case 4:
        uVar2 = 4;
        break;
    case 5:
        uVar2 = 5;
    }
    *puVar1 = uVar2;
    if (puVar1[0x10] == 0) {
        func_0206f324();
        func_0206f4dc();
        func_0203140c();
    }
    else {
        func_02031420((unsigned char *)0xfffffff0);
        func_02031430((unsigned char *)0xfffffff0);
    }
    func_0206f544(2);
    func_02030378();
    func_0201f038();
    func_02031440();
    func_02031450();
    func_02031460();
    func_0206f494(1, (unsigned int)(func_02073480(1, 0x02031561) >> 32));
    func_020735c0(1);
    func_02031440();
    func_0202286c();
}
