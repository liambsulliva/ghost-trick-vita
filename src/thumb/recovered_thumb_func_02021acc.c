extern int func_02020e54();
extern int func_02021b34();

unsigned int func_02021acc(unsigned int param_1,unsigned int param_2,int param_3)

{
    int iVar1;
    unsigned int uVar2;

    iVar1 = func_02021b34(param_2);
    if (iVar1 == 0) {
        return 0;
    }
    if (param_3 == 1) {
        uVar2 = func_02020e54(param_2);
        if ((uVar2 & 0x100) == 0) {
            return 0;
        }
    }
    if (param_3 == 2) {
        uVar2 = func_02020e54(param_2);
        if ((uVar2 & 0x200) == 0) {
            return 0;
        }
    }
    if (((param_3 == 0) && (uVar2 = func_02020e54(param_2), (uVar2 & 0x400) == 0)) &&
          (uVar2 = func_02020e54(param_2), (uVar2 & 0x300) != 0)) {
        return 0;
    }
    return 1;
}
