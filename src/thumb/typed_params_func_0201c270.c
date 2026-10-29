extern unsigned char * func_02009900();
extern int func_0201c294();
extern int func_0202f684();
extern int func_0202f7e8();

int func_0201c270(int param_1,int param_2)

{
    int iVar1;
    unsigned char *puVar2;

    iVar1 = func_0202f684(param_1);
    if (iVar1 != 0) {
        func_0201c294(iVar1,param_2);
        puVar2 = func_02009900();
        func_0202f7e8((int)puVar2,iVar1,0xd,0);
    }
    return iVar1;
}
