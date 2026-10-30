extern int func_02013514();
extern int func_020790c8();
extern int func_02008444();
extern void func_02011110();
extern void func_020112b4();
extern char data_02084110;
extern char data_0208411c;

void func_020134d4(int param_1, int param_2, char *param_3)
{
    int uVar1;
    int iVar2;
    int *puVar3;

    uVar1 = func_02013514(param_3);
    iVar2 = func_020790c8(uVar1, &data_02084110);
    if (iVar2 == 0) {
        puVar3 = (int *)func_02008444();
        func_02011110(puVar3, 1);
        return;
    }
    iVar2 = func_020790c8(uVar1, &data_0208411c);
    if (iVar2 == 0) {
        puVar3 = (int *)func_02008444();
        func_020112b4(puVar3);
    }
    return;
}
