extern char data_020b42d8;
extern unsigned char * func_02009800();
extern unsigned char * func_02009900();
extern unsigned char * func_02009c1c();
extern int func_0200a248();
extern int func_0200a254();
extern int func_0202f3a4();
extern int func_0202f7e8();

void func_0200a200(unsigned char *param_1)
{
    unsigned char *puVar1;
    int iVar2;
    unsigned char *puVar3;

    func_0200a248(param_1,1);
    puVar1 = func_02009c1c();
    iVar2 = func_0200a254(puVar1,0x10);
    *(int *)(param_1 + ((int)0x0000aa44u)) = iVar2;
    puVar1 = func_02009c1c();
    func_0202f3a4((int)puVar1,0x10,0);
    puVar1 = func_02009800(&data_020b42d8);
    if (puVar1 != (unsigned char *)0x0) {
        puVar3 = func_02009900();
        func_0202f7e8((int)puVar3,(int)puVar1,0xd,0);
    }
    return;
}
