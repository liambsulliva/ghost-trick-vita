extern char data_0209c108;
typedef int code();
extern int func_02007b70();
extern int func_02007bd8();
extern int func_020086ac();
extern int func_0201ef58();
extern int func_0201ef74();
extern int func_0201efb4();
extern int func_0201f014();
extern int func_0201f02c();
extern int func_02051dbc();
extern int func_02073174();
extern int func_02074860();

int func_02051d44(unsigned int param_1,unsigned int param_2,unsigned int param_3,unsigned int param_4)

{
    unsigned int uVar1;
    int iVar2;
    unsigned int uVar3;
    int *piVar4;
    int iVar5;
    unsigned char auStack_68 [80];
    unsigned int uStack_18;

    iVar5 = 0;
    uStack_18 = param_4;
    func_0201ef58(auStack_68);
    func_02007bd8();
    uVar1 = func_020086ac();
    iVar2 = func_0201efb4(auStack_68,param_1,1);
    if (iVar2 != 0) {
        uVar3 = func_0201f02c(auStack_68);
        func_0201f014(auStack_68,uVar1,uVar3);
        uVar3 = func_02051dbc(uVar1);
        func_02007bd8();
        piVar4 = (int *)func_02007b70();
        iVar5 = (**(code **)(*piVar4 + 8))(piVar4,uVar3,((unsigned int)&data_0209c108));
        if (iVar5 != 0) {
            func_02074860(uVar1,uVar3);
            func_02073174(uVar1,iVar5);
        }
    }
    func_0201ef74(auStack_68);
    return iVar5;
}
