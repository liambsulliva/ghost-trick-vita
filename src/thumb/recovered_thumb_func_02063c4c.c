extern int func_0202f644();
extern int func_02056cac();
extern int func_020716e8();
extern int func_020719fc();

unsigned int * func_02063c4c(unsigned int *param_1)

{
    *param_1 = ((unsigned int)0x020afed8u);
    if (*(char *)((int)param_1 + 0xc6) != '\0') {
        func_020716e8(4);
        func_020719fc(8);
    }
    func_02056cac(param_1);
    func_0202f644(param_1);
    return param_1;
}
