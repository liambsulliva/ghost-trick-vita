extern int data_020b9d04;
extern void func_0201003c();
extern void func_0200b518();
extern void func_0200b598();
extern void func_02008398();
extern void func_020083b0();
extern void func_020280a8();
extern void func_0203fdd8();

void func_02054c90(int param_1)
{
    int local[3];

    func_0201003c(param_1);
    func_0200b518(param_1 + 0x28,0,0x80000000,data_020b9d04);
    func_0200b598(param_1 + 0x28,2,0xf);
    func_02008398(local,0xf6000,0xb7000,0);
    func_020083b0(param_1 + 0xb0,local);
    func_020280a8(param_1 + 0x28,0);
    func_0203fdd8(param_1 + 0x28,0);
}
