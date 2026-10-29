extern unsigned int * func_02008444();
extern unsigned char func_02010b54();
extern int func_02029640();
extern int func_0203435c();
extern int func_020570e4();

void func_0205c458(int param_1)

{
    unsigned int *puVar2;
    int iVar3;

    puVar2 = func_02008444();
    iVar3 = func_0203435c((unsigned char *)puVar2);
    if (iVar3 == 0) {
        iVar3 = func_02029640(param_1);
        if (iVar3 == 0 || func_02010b54(iVar3) != 0) {
            func_020570e4((int *)param_1);
        }
    }
    return;
}
