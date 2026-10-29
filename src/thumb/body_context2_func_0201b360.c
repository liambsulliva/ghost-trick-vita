extern unsigned int data_020c9e48;
extern int func_020164a8();
extern int func_020164c4();

void func_0201b360(unsigned char *param_1,int param_2)

{
    int iVar1;

    func_020164a8(param_1,param_2);
    iVar1 = func_020164c4(param_1);
    if (iVar1 == 0) {
        data_020c9e48 = ((unsigned int)0xfffffdffu) & data_020c9e48;
    }
    return;
}
