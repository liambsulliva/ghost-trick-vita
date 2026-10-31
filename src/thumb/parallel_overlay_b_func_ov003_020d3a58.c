extern int func_ov003_020d50ec();
extern int func_ov003_020d5108();
extern int func_ov003_020d5298();
extern int func_ov003_020d5350();
extern int func_ov003_020d5360();
extern int func_ov003_020d5374();
extern int func_ov003_020d5470();
extern int func_ov003_020d5498();
extern int func_ov003_020d5540();
extern int func_ov003_020d5584();
extern int func_ov003_020d55c8();
extern int func_ov003_020d5634();

void func_ov003_020d3a58(int param_1)
{
    unsigned char cVar1;

    cVar1 = *(unsigned char *)(param_1 + 0x168);
    if (cVar1 == 0) {
        func_ov003_020d50ec(param_1);
        return;
    }
    if (cVar1 == 1) {
        func_ov003_020d5108(param_1);
        return;
    }
    if (cVar1 == 2) {
        func_ov003_020d5298(param_1);
        return;
    }
    if (cVar1 == 3) {
        func_ov003_020d5350(param_1);
        return;
    }
    if (cVar1 == 4) {
        func_ov003_020d5360(param_1);
        return;
    }
    if (cVar1 == 5) {
        func_ov003_020d5374(param_1);
        return;
    }
    if (cVar1 == 6) {
        func_ov003_020d5470(param_1);
        return;
    }
    if (cVar1 == 7) {
        func_ov003_020d5498(param_1);
        return;
    }
    if (cVar1 == 8) {
        func_ov003_020d5540(param_1);
        return;
    }
    if (cVar1 == 9) {
        func_ov003_020d5584(param_1);
        return;
    }
    if (cVar1 == 10) {
        func_ov003_020d55c8(param_1);
        return;
    }
    if (cVar1 == 11) {
        func_ov003_020d5634(param_1);
    }
    return;
}
