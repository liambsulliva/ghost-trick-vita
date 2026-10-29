extern unsigned char * func_02009900();
extern int func_0202f7e8();

void func_0202fd3c(int param_1,int param_2)

{
    unsigned char *puVar1;
    unsigned int uVar2;

    uVar2 = *(unsigned int *)(param_1 + 0xc);
    if ((uVar2 != 0) && (param_2 != 0)) {
        puVar1 = func_02009900();
        func_0202f7e8((int)puVar1,param_2,uVar2,1);
    }
    return;
}
