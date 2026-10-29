extern signed char data_020cfc14;
extern signed char data_020cfc30;
typedef int code();
extern int func_02009900();
extern int func_020153d8();
extern int func_0202f64c();
extern int func_0202f7e8();
extern int func_0204c540();
extern int func_02061cd8();
extern int func_02061cec();
extern int func_02062e40();
extern int func_02062e48();

void func_02062d8c(int param_1,int param_2)

{
    unsigned int uVar1;
    int *piStack_10;

    piStack_10 = *(int **)(param_1 + 0xf4);
    if (param_2 == 0) {
        uVar1 = func_02061cec(((unsigned int)&data_020cfc30));
    }
    else {
        uVar1 = func_02061cd8(((unsigned int)&data_020cfc14));
    }
    *(unsigned int *)(param_1 + 0xf4) = uVar1;
    (**(code **)(**(int **)(param_1 + 0xf4) + 0x8c))
                        (*(int **)(param_1 + 0xf4),(int)(signed char)(*(int *)(param_1 + 0x84) >> 0xc));
    func_020153d8(*(unsigned int *)(param_1 + 0xf4),param_1);
    uVar1 = func_02009900();
    func_0202f7e8(uVar1,*(unsigned int *)(param_1 + 0xf4),7,0);
    if (piStack_10 != (int *)0x0) {
        uVar1 = func_02062e40();
        func_0204c540(*(unsigned int *)(param_1 + 0xf4),uVar1);
        uVar1 = (**(code **)(*piStack_10 + 0x88))(piStack_10);
        (**(code **)(**(int **)(param_1 + 0xf4) + 0x84))(*(int **)(param_1 + 0xf4),uVar1);
        func_0202f64c(&piStack_10);
    }
    if (param_2 == 0) {
        func_02062e48(param_1,0);
        return;
    }
    func_02062e48(param_1,1);
    return;
}
