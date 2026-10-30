class Object {
public:
    virtual void slot0();
    virtual void slot1();
    virtual void slot2();
    virtual void slot3();
    virtual unsigned int slot4();
};
extern "C" {
extern char data_020cddf8;
extern char data_020cde84;
extern char data_020cf75c;

int func_02008314(void *param_1, void *param_2);
unsigned int *func_02008444(void);
void func_0202f64c(void *param_1);
void func_0200f084(unsigned int *param_1, int param_2, unsigned int param_3, int param_4, int param_5);
void func_0200e930(unsigned int *param_1, unsigned int param_2);
unsigned int func_0203bb88(unsigned int param_1, int param_2);
void func_0200ef08(unsigned int *param_1);
void func_0202fbb0(unsigned char *param_1);
int func_0200eac4(unsigned char *param_1);
unsigned char *func_0200eab4(unsigned char *param_1);
unsigned char *func_0200eab8(unsigned char *param_1);
int func_02008828(unsigned char *param_1, unsigned char *param_2);
void func_0203bba0(unsigned char *param_1);
void func_0200eabc(unsigned char *param_1, int param_2);

void func_0203baac(Object *param_1, unsigned int param_2, unsigned int param_3, unsigned int param_4)
{
    unsigned char *puVar2;
    int iVar3;
    unsigned int *puVar4;
    unsigned int uVar8;
    unsigned char *puVar6;
    unsigned char auStack_24[16];
    unsigned int uStack_14;

    uStack_14 = param_4;
    func_0202f64c((void *)((int *)param_1 + 8));
    func_0202f64c((void *)((int *)param_1 + 10));
    puVar2 = (unsigned char *)param_1->slot4();
    iVar3 = func_02008314(puVar2, (void *)&data_020cddf8);
    if (iVar3 != 0) {
        puVar4 = func_02008444();
        func_0200f084(puVar4, 0, *(unsigned char *)((int)param_1 + 0x2d), 1, 0);
    }
    else {
        puVar4 = func_02008444();
        func_0200f084(puVar4, 0, *(unsigned char *)((int)param_1 + 0x2d), 1, 1);
    }
    uVar8 = (unsigned int)((int *)param_1)[0xd];
    if (uVar8 != 0) {
        puVar4 = func_02008444();
        func_0200e930(puVar4, uVar8 & 0xff);
    }
    uVar8 = param_1->slot4();
    uVar8 = func_0203bb88(uVar8, (int)&data_020cde84);
    if (uVar8 != 0) {
        uVar8 = param_1->slot4();
        if (func_0203bb88(uVar8, (int)&data_020cddf8) != 0) {
            puVar4 = func_02008444();
            func_0200ef08(puVar4);
        }
    }
    func_0202fbb0(auStack_24);
    puVar2 = (unsigned char *)&data_020cf75c;
    while (iVar3 = func_0200eac4(auStack_24), iVar3 != 0) {
        puVar6 = func_0200eab4(auStack_24);
        iVar3 = func_02008828(puVar6, puVar2);
        if (iVar3 != 0) {
            puVar6 = func_0200eab8(auStack_24);
            func_0203bba0(puVar6);
        }
        func_0200eabc(auStack_24, 0);
    }
}
}
