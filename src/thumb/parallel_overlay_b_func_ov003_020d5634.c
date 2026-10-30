extern int func_0201123c();
extern int func_02011264();
extern int func_0201572c();
extern int func_02022dfc();
extern int func_02022f34();
extern int func_ov003_020d4818();
extern unsigned char data_ov003_020d5b14[];
extern unsigned char data_ov003_020d5b20[];

void func_ov003_020d5634(int param_1)
{
    if (func_0201572c() == 0) {
        func_02022f34(func_0201123c(func_02011264(), 0), data_ov003_020d5b14, 0);
        func_02022f34(func_02022dfc(param_1), data_ov003_020d5b20, 0);
    }
    func_ov003_020d4818(param_1);
}
