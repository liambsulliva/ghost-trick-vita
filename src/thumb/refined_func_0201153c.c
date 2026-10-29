extern int func_0200fbac();
extern int func_02011578();
extern int func_020114d4();

void func_0201153c(int param_1,int param_2)

{
    func_02011578(param_1 + ((int)0x00000ae4u),0x7e);
    if ((unsigned int)param_2 < 1) {
        func_0200fbac(param_1 + 100,0x80);
        return;
    }
    func_020114d4(param_1 + 0x164 + param_2 * 0x80,param_1 + 100,0x80);
    return;
}
