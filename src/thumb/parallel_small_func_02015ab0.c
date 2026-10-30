extern unsigned int * func_02008444();
extern unsigned char * func_02009900();
extern unsigned char * func_0200e8b0();
extern int func_02014a7c();
extern int func_02015970();
extern int func_02015990();
extern unsigned char * func_02015afc();
extern unsigned int * func_0202f624();
extern int func_0202f7e8();
extern unsigned int * func_02042d90();

void func_02015ab0(unsigned char *param_1)

{
    unsigned int *puVar1;
    unsigned int *puVar2;
    unsigned char *puVar3;

    puVar1 = func_0202f624(0xa8);
    puVar2 = puVar1;
    if (puVar1 != (unsigned int *)0x0) {
        puVar3 = func_02015afc(param_1);
        puVar2 = func_02042d90(puVar1,puVar3);
    }
    *(unsigned int **)(param_1 + 8) = puVar2;
    puVar3 = func_02009900();
    func_0202f7e8((int)puVar3,*(int *)(param_1 + 8),1,0);
    func_02015970(param_1);
    puVar2 = func_02008444();
    puVar3 = func_0200e8b0((unsigned char *)puVar2);
    func_02014a7c(puVar3);
    func_02015990(param_1,1);
    return;
}
