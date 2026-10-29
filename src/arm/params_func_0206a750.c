extern unsigned char func_0206a120();

unsigned char func_0206a750(unsigned int *param_1)

{
    unsigned char bVar1;

    func_0206a120(param_1,2,1);
    *(unsigned int *)(*param_1 + 0xc) = 0;
    *(unsigned int *)(*param_1 + 0x10) = ((unsigned int)0x020d07e0u);
    *(unsigned int *)(*param_1 + 0x14) = 1;
    bVar1 = func_0206a120(param_1,6,1);
    return bVar1;
}
