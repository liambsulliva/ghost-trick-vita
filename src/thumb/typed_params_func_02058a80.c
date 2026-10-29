extern int func_02008378();
extern int func_02008398();
extern unsigned char * func_020083a0();
extern unsigned char func_02010b54();

void func_02058a80(unsigned char *param_1,int param_2)

{
    unsigned char bVar1;
    unsigned char *puVar2;

    bVar1 = func_02010b54(*(int *)(param_2 + 4));
    if (bVar1) {
        puVar2 = func_020083a0(*(unsigned char **)(param_2 + 4));
        func_02008378(param_1,puVar2,(unsigned char *)(param_2 + 8));
        return;
    }
    func_02008398(param_1,((int)0xffc00000u),0,0);
    return;
}
