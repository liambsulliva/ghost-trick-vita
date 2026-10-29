extern int func_020514d4();

int func_020514b8(int param_1,int param_2)

{
    func_020514d4();
    *(unsigned int *)(param_1 + 4) = *(unsigned int *)(param_2 + 4);
    *(unsigned int *)(param_1 + 8) = *(unsigned int *)(param_2 + 8);
    *(unsigned int *)(param_1 + 0xc) = *(unsigned int *)(param_2 + 0xc);
    return param_1;
}
