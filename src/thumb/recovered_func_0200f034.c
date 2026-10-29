typedef int code();
extern int func_02008860();
extern int func_0200b00c();

unsigned int func_0200f034(int param_1)

{
    unsigned int uVar1;
    int *piVar2;
    unsigned int uVar3;

    uVar1 = func_02008860();
    uVar3 = 0;
    piVar2 = (int *)func_0200b00c(uVar1,0);
    if (piVar2 != (int *)0x0) {
        uVar3 = (**(code **)(*piVar2 + 8))(piVar2,*(unsigned int *)(param_1 + ((int)0x00000efcu)));
    }
    return uVar3;
}
