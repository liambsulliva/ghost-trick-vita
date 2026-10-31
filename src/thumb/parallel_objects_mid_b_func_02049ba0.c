extern int func_020082d8();
extern void *func_02008444();
extern int func_02049bdc();
extern int func_02049be8();
typedef int code();

void func_02049ba0(int param_1)
{
    int aiStack[3];
    void *puVar1;
    int iVar2;

    func_020082d8(aiStack, param_1 + 0xf0);
    puVar1 = func_02008444();
    iVar2 = func_02049bdc(puVar1);
    aiStack[0] = aiStack[0] - iVar2;
    puVar1 = func_02008444();
    iVar2 = func_02049be8(puVar1);
    aiStack[1] = aiStack[1] + iVar2;
    (**(code **)(**(int **)(param_1 + 0xb8) + 0x30))(*(int **)(param_1 + 0xb8), aiStack);
    return;
}
