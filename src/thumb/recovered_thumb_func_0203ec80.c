extern int func_0200aca8();
extern int func_02022d70();
extern int func_0203ece0();

unsigned int * func_0203ec80(unsigned int *param_1)

{
    func_02022d70();
    *param_1 = ((unsigned int)0x020a75c8u);
    param_1[8] = 0;
    func_0200aca8((int)param_1 + ((int)0x00000408u));
    func_0203ece0((int)param_1 + ((int)0x00000668u));
    return param_1;
}
