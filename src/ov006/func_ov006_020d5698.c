extern int func_0207dc50();
extern int func_0207dea4();
extern int func_ov006_020d5680();

void func_ov006_020d5698(unsigned int *param_1,unsigned int param_2)

{
    unsigned int uVar1;
    unsigned int uVar2;

    if (*param_1 < param_2) {
        uVar1 = func_0207dc50(param_2,0xc,8,((unsigned int)0x020d5629u),((unsigned int)0x0203ed0du));
        for (uVar2 = 0; uVar2 < *param_1; uVar2 = uVar2 + 1) {
            func_ov006_020d5680(uVar1 + uVar2 * 0xc,param_1[2] + uVar2 * 0xc);
        }
        if (param_1[2] != 0) {
            func_0207dea4(param_1[2],0xc,8,((unsigned int)0x0203ed0du));
            param_1[2] = 0;
        }
        param_1[2] = uVar1;
        param_1[1] = param_2;
    }
    return;
}
