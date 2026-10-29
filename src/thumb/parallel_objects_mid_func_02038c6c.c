extern int func_020229b4();
extern int func_02037aa4();
extern int func_02038afc();

void func_02038c6c(int param_1)

{
    unsigned int uVar1;

    uVar1 = func_02038afc(*(unsigned char **)(param_1 + 0xc),(int)*(short *)(param_1 + 10));
    if (uVar1 != 0) {
        switch(*(short *)(param_1 + 10)) {
        case 0:
            func_02037aa4(3);
            return;
        case 1:
            func_02037aa4(1);
            return;
        case 2:
            func_020229b4(0x1000);
            return;
        case 3:
            func_02037aa4(2);
            return;
        case 4:
            func_02037aa4(4);
        }
    }
    return;
}
