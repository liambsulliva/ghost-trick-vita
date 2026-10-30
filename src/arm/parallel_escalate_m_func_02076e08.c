typedef struct {
    unsigned int unk0;
    unsigned int unk4;
} Info02076e08;

extern Info02076e08 data_020d1344;
extern int func_02075c98(void);
extern void func_02075cac(int);

int func_02076e08(unsigned int param_1)
{
    int enabled;
    unsigned int cur;
    int ok;

    enabled = func_02075c98();
    cur = data_020d1344.unk4;
    if (param_1 > cur) {
        if ((param_1 - cur) < 0x80000000) {
            ok = 0;
        } else {
            ok = 1;
        }
    } else {
        ok = 1;
        if ((cur - param_1) >= 0x80000000) {
            ok = 0;
        }
    }
    func_02075cac(enabled);
    return ok;
}
