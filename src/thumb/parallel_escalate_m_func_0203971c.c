extern unsigned short data_0209a754[2];
extern unsigned short data_0209a7cc[24][3];
extern void func_020379f0(unsigned char *object, int value);

void func_0203971c(unsigned char *param_1, int param_2)
{
    if (param_1[0xe4] != 0) {
        if (param_1[0xdc] < 2) {
            func_020379f0(param_1, param_2 + data_0209a754[param_1[0xdc]]);
        }
    } else {
        if (param_1[0xdd] < 0x18 && param_1[0xe2] < 3) {
            func_020379f0(param_1, param_2 + data_0209a7cc[param_1[0xdd]][param_1[0xe2]]);
        }
    }
}
