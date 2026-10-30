typedef int code();
extern unsigned char func_02010b54();

unsigned int func_0205b9e8(int param_1,unsigned int param_2,unsigned int param_3)

{
    unsigned char bVar1;
    int iVar2;

    if (((*(int *)(param_1 + 0x28) != 0) && (bVar1 = func_02010b54(*(int *)(param_1 + 0x28)), bVar1))
          && (iVar2 = (**(code **)(**(int **)(param_1 + 0x28) + 0xbc))(*(int **)(param_1 + 0x28),(unsigned short)param_2,(unsigned short)param_3), iVar2 != 0)) {
        return 1;
    }
    return 0;
}
