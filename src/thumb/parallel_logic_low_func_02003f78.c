struct State {
    unsigned int before[4];
    int data;
};
extern struct State data_020b18a0;
extern int func_02003e30();
extern int func_02007948();

void func_02003f78(int param_1,int param_2)

{
    int iVar1;
    int iVar2;
    int iVar3;

    iVar1 = ((int)0x020b18a0u);
    iVar3 = 0;
    do {
        iVar2 = data_020b18a0.data + ((int)0x000015bcu) + iVar3 * 0x4c;
        if (*(int *)(iVar2 + 4) == param_1) {
            func_02003e30(iVar2,param_2,0);
        }
        iVar3 = iVar3 + 1;
    } while (iVar3 < 0x10);
    func_02007948();
    return;
}
