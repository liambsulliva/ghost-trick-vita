extern int func_02008444();
extern int func_02010384();
extern int func_0200da04();
extern int func_020113f8();
extern int func_02032644();
extern int func_02046490();
extern int func_02027458();
extern int func_02010718();
extern int func_02010b74();
extern int func_02031af4();
extern int func_0202f64c();
extern int func_02031b04();
extern unsigned char data_0209b634;

void func_020463e4(unsigned char *param_1)
{
    int iVar2;

    if (param_1[199] == 0) {
        iVar2 = *(int *)(param_1 + 0xd0);
        if (iVar2 == 0) {
            func_02010384(func_02008444(), 0);
            if (*(int *)(param_1 + 0x168) != 0) {
                func_0200da04(*(int *)(param_1 + 0x168), *(int *)(param_1 + 0x16c), *(int *)(param_1 + 0x170), (int)&data_0209b634);
                func_020113f8(func_02008444());
                func_02032644(func_02008444(), 4);
            }
            else {
                func_02046490(func_02008444());
                func_02027458(*(unsigned int *)(param_1 + 0x164));
            }
        }
        else {
            func_02010718(func_02008444(), iVar2);
        }
        func_02010b74(param_1);
        *(int *)(param_1 + 0x10) = 4;
        if (*(int *)(param_1 + 0x158) != 0) {
            iVar2 = *(int *)(param_1 + 0x160);
            if (iVar2 >= 0) {
                if (func_02031af4(func_02008444(), iVar2) == 0) {
                    func_0202f64c(param_1 + 0x158);
                    func_02031b04(func_02008444(), param_1 + 0x160);
                }
            }
        }
    }
}
