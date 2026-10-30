extern int func_0200f70c();
extern int func_020266dc();
extern void func_02058a50();

void func_02059170(int param_1, int param_2, int param_3, int param_4)
{
    int aiStack_c[3];
    int iVar1;

    func_0200f70c(aiStack_c);
    aiStack_c[2] = 0;
    iVar1 = func_020266dc(param_1, param_2);
    if (iVar1 != 0) {
        aiStack_c[0] = (int)((short)param_3 << 12);
        aiStack_c[1] = (int)((short)param_4 << 12);
        func_02058a50(iVar1, aiStack_c);
    }
}
