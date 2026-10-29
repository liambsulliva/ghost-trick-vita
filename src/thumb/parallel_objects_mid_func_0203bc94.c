extern unsigned char * func_020082f8();
extern unsigned int * func_02008444();
extern int func_02010e5c();
extern int func_02010e7c();
extern int func_0201132c();
extern int func_0201135c();
extern int func_0201192c();
extern int func_02022dd8();
extern int func_0203baa4();
extern int func_0203bd10();

void func_0203bc94(unsigned char *param_1)

{
    unsigned int *puVar1;
    int iVar2;
    unsigned char *puVar3;

    func_0203baa4(param_1);
    puVar1 = func_02008444();
    iVar2 = func_0201132c((int)puVar1);
    if (iVar2 != 0) {
        puVar1 = func_02008444();
        func_0201192c((int)puVar1,0x16);
        puVar3 = (unsigned char *)func_02022dd8((int)param_1);
        puVar3 = func_020082f8(puVar3,((unsigned int)0x020cd96cu));
        if ((puVar3 == (unsigned char *)0x0) || (iVar2 = func_0203bd10(puVar3), iVar2 == 0)) {
            puVar1 = func_02008444();
            func_02010e7c((unsigned char *)puVar1);
        }
    }
    else {
        puVar1 = func_02008444();
        func_0201192c((int)puVar1,4);
        puVar3 = (unsigned char *)func_02022dd8((int)param_1);
        puVar3 = func_020082f8(puVar3,((unsigned int)0x020cd7ecu));
        if ((puVar3 == (unsigned char *)0x0) || (iVar2 = func_0203bd10(puVar3), iVar2 == 0)) {
            puVar1 = func_02008444();
            func_02010e5c((unsigned char *)puVar1);
        }
    }
    puVar1 = func_02008444();
    func_0201135c((int)puVar1);
    return;
}
