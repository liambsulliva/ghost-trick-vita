typedef void (*TPCallback)(int command, int result, int arg);

struct TPState {
    unsigned int pad0;
    TPCallback callback;
    unsigned char mid[48];
    unsigned short err_flg;
    unsigned short command_flg;
};

extern struct TPState data_020d2e84;
extern unsigned int func_02075c98(void);
extern unsigned int func_02075cac(unsigned int bits);
extern int func_02076240(int tag, unsigned int data, int err);

void func_02077bc0(void)
{
    unsigned int bak;
    int ok;

    bak = func_02075c98();
    ok = 0;
    if (func_02076240(6, 0x3000000, 0) >= 0) {
        ok = 1;
    }
    if (!ok) {
        func_02075cac(bak);
        data_020d2e84.err_flg |= 1;
        if (data_020d2e84.callback) {
            data_020d2e84.callback(0, 4, 0);
        }
        return;
    }
    data_020d2e84.command_flg |= 1;
    data_020d2e84.err_flg &= ~1;
    func_02075cac(bak);
}
