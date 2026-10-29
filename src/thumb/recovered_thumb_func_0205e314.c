extern int func_02056ac8();

unsigned int * func_0205e314(unsigned int *param_1)

{
    func_02056ac8();
    *param_1 = ((unsigned int)0x020af3a4u);
    param_1[0x32] = 0;
    *(unsigned char *)((int)param_1 + 0xcd) = 1;
    *(unsigned char *)((int)param_1 + 0xce) = 1;
    return param_1;
}
