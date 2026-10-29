extern int func_0200f70c();
extern int func_0202b1f4();
extern int func_0202b2e0();

unsigned int * func_0202c3f8(unsigned int *param_1)

{
    unsigned int *puVar1;

    func_0202b2e0();
    *param_1 = ((unsigned int)0x020a5148u);
    *(unsigned short *)(param_1 + 0x21) = 0;
    puVar1 = param_1 + 0x28;
    do {
        func_0200f70c(puVar1);
        puVar1 = puVar1 + 3;
    } while (puVar1 < param_1 + 0x34);
    func_0202b1f4(param_1 + 0x3d);
    param_1[0x70] = 0;
    *(unsigned char *)((int)param_1 + 5) = 2;
    return param_1;
}
