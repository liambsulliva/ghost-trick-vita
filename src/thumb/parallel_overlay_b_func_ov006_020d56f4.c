extern int func_ov006_020d5760();
extern int func_02008444();
extern int func_0200fbd4();
extern int func_ov006_020d4054();
extern int func_ov006_020d5778();
extern int func_ov006_020d420c();
extern unsigned char data_020c9e48[];
extern unsigned char data_020c9e88[];

void func_ov006_020d56f4(unsigned char *param_1, int param_2)
{
    func_ov006_020d5760(1);
    *(unsigned int *)(data_020c9e48 + 0x54) = (*(unsigned int *)(data_020c9e48 + 0x54) & 0x3ffffdff) | 0xc0001800;
    *(short *)(data_020c9e88 + 0x1e) = 0x143;
    param_1[0x25] = 0;
    func_0200fbd4(func_02008444(), 3);
    func_ov006_020d4054(param_2);
    func_ov006_020d5778(param_2, *(signed char *)(param_1 + *(signed char *)(param_1 + 0x32) + 0x38));
    func_ov006_020d420c(param_2);
    param_1[0x24] = 4;
}
