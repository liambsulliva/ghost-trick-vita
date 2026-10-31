extern int func_02007108();
extern int func_02007120();

unsigned int func_0202a7bc(unsigned int param_1,unsigned char *param_2)

{
    int iVar1;
    int iVar2;

    iVar1 = func_02007108(param_2);
    iVar2 = func_02007120(param_2);
    if (iVar2 == 0) {
        return 2;
    }
    if (iVar2 == 1) {
        return 3;
    }
    if (iVar2 == 2) {
        return 4;
    }
    if (iVar2 == 3) {
        if (iVar1 == 1) {
            return 5;
        }
        switch (iVar1) {
        case 2:
            return 0;
        }
        return 1;
    }
    return 0;
}
