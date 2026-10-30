struct Owner {
    unsigned char before[0x24];
    unsigned int value;
    unsigned char middle[0x10];
    unsigned int slots[2];
    unsigned int gap;
    unsigned int last;
    unsigned char pad;
    signed char index;
};
extern char data_0209a9c4;
extern unsigned int * func_02008444();
extern unsigned char * func_02009900();
extern int func_02010ef4();
extern int func_0201192c();
extern int func_0202fb8c();
extern int func_02061230();

void func_0203bd30(struct Owner *param_1)

{
    unsigned int *puVar1;
    unsigned char *puVar2;
    int iVar3;
    unsigned int uVar4;

    puVar1 = func_02008444();
    func_02010ef4((int)puVar1,0xff,-1);
    puVar1 = func_02008444();
    func_0201192c((int)puVar1,0x15);
    puVar2 = func_02009900();
    iVar3 = func_0202fb8c(puVar2,((int)&data_0209a9c4),0xffffffff);
    if (iVar3 != 0) {
        func_02061230(iVar3);
    }
    uVar4 = 0;
    do {
        param_1->slots[uVar4] = 0;
        uVar4 = uVar4 + 1;
    } while (uVar4 < 2);
    param_1->value = 0;
    param_1->last = 0;
    param_1->index = -1;
    return;
}
