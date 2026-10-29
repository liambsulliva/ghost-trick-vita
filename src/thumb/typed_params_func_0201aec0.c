extern int func_0201af50();
extern int func_0201b044();
extern int func_0201b0b8();

void func_0201aec0(unsigned char *param_1,int unused_2,unsigned int param_3,unsigned int param_4)

{
    int iVar1;




    iVar1 = *(int *)(param_1 + 0x54);
    if (iVar1 != 1) {
        if (iVar1 == 2) {
            param_1[0x4e] = param_1[0x4e] + '\x01';
            return;
        }
        func_0201af50((int)param_1,iVar1,param_3,param_4);
        func_0201b044((int)param_1);
        func_0201b0b8(param_1);
        param_1[0x3b] = param_1[0x3b] + '\x01';
    }
    return;
}
