extern char data_020b03a0;
extern char data_020c9e48;
extern char data_020c9e88;
typedef int code();
extern int func_02007b70();
extern int func_02007bd8();
extern int func_020644b0();

unsigned int * func_02064534(unsigned int *param_1)

{
    unsigned int *puVar1;
    int iVar2;
    int *piVar3;

    puVar1 = ((unsigned int *)&data_020c9e48);
    *param_1 = ((unsigned int)&data_020b03a0);
    *puVar1 = param_1[0x12];
    *(unsigned short *)((int)puVar1 + 0x1a) = *(unsigned short *)(param_1 + 0x14);
    puVar1[0x15] = param_1[0x13];
    *(unsigned short *)(((int)&data_020c9e88) + 0x26) = *(unsigned short *)((int)param_1 + 0x52);
    if (param_1[0xe] != 0) {
        func_02007bd8();
        iVar2 = func_02007b70();
        if (iVar2 != 0) {
            func_02007bd8();
            piVar3 = (int *)func_02007b70();
            (**(code **)(*piVar3 + 0x10))(piVar3,param_1[0xe]);
            param_1[0xe] = 0;
        }
    }
    if (param_1[0xf] != 0) {
        func_02007bd8();
        iVar2 = func_02007b70();
        if (iVar2 != 0) {
            func_02007bd8();
            piVar3 = (int *)func_02007b70();
            (**(code **)(*piVar3 + 0x10))(piVar3,param_1[0xf]);
            param_1[0xf] = 0;
        }
    }
    func_020644b0(param_1);
    return param_1;
}
