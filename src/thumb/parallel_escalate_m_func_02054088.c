extern int func_0200b518();
extern int func_0200b598();
extern void func_020280a8();
extern void func_0203fdd8();

void func_02054088(unsigned char *param_1)

{
    func_0200b518(param_1 + 0x28,0,0x80000000,0x80000000);
    func_0200b598(param_1 + 0x28,1,0xf);
    func_020280a8(param_1 + 0x28,0);
    func_0203fdd8(param_1 + 0x28,1);
    *(unsigned int *)(param_1 + 0xbc) = 0;
    *(unsigned int *)(param_1 + 0xc0) = 0xffffffff;
    return;
}
