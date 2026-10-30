void func_02075f84(void);

void func_02072e3c(int param_1, unsigned int param_2, unsigned int param_3, unsigned int param_4)
{
    unsigned int high;
    unsigned int low;

    if (param_1 != 0) {
        return;
    }
    high = param_2 & 0xff000000;
    if (param_4 != 0) {
        if (param_4 == 0x800000) {
            param_2 = param_2 - param_3;
        }
    } else {
        param_2 = param_2 + param_3;
    }
    low = param_2 & 0xff000000;
    if (high == 0x4000000 || high >= 0x8000000 || low == 0x4000000 || low >= 0x8000000) {
        func_02075f84();
    }
}
