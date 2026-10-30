extern int func_02011264();
extern int func_0201123c();
extern int func_02022f34();
extern int func_02022f4c();
extern int func_02008444();
extern int func_0201135c();
extern unsigned char data_ov006_020d8e00[];
extern unsigned char data_ov006_020d8e0c[];
extern unsigned char data_ov006_020d8e1c[];

void func_ov006_020d5928(int param_1)
{
    if (*(int *)(param_1 + 0x20) == 1) {
        func_02022f34(func_0201123c(func_02011264(), 0), data_ov006_020d8e00, 0);
        func_0201123c(func_02011264(), 1);
        func_02022f4c();
        return;
    }
    func_02022f34(func_0201123c(func_02011264(), 0), data_ov006_020d8e0c, 0);
    func_02022f34(func_0201123c(func_02011264(), 1), data_ov006_020d8e1c, 0);
    func_02008444();
    func_0201135c();
}
