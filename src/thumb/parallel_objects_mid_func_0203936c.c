extern int func_02039294();
extern int func_020392b0();

void func_0203936c(int param_1)

{
    switch(*(signed char *)(param_1 + 0xde)) {
    case -1:
        func_02039294(param_1,0x24000);
        func_020392b0(param_1,((int)0xfffa8000u));
        return;
    case 0:
        func_02039294(param_1,0x14000);
        func_020392b0(param_1,((int)0xfffc4000u));
        return;
    case 1:
        func_02039294(param_1,0x8000);
        func_020392b0(param_1,((int)0xfffe0000u));
        return;
    case 3:
        func_02039294(param_1,0x8000);
        func_020392b0(param_1,0x20000);
        return;
    case 4:
        func_02039294(param_1,0x14000);
        func_020392b0(param_1,0x3c000);
        return;
    case 5:
        func_02039294(param_1,0x24000);
        func_020392b0(param_1,0x58000);
        return;
    }
    return;
}
