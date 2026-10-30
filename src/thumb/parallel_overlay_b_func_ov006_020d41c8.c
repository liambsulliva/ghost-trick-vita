int func_ov006_020d41c8(int param_0, int param_1, int param_2, unsigned int param_3)
{
    if (param_1 != 0) {
        if (param_2 == 0xf && param_3 >= 0x14) {
            return 0x17;
        }
        if (param_2 == 0x14 && param_3 >= 0x3c) {
            return 0x18;
        }
    }
    else {
        if (param_2 == 0x11 && param_3 >= 0x8c) {
            return 0x12;
        }
        if (param_2 == 2 && param_3 >= 0x96) {
            return 6;
        }
        if (param_2 == 0x28 && param_3 < 0x14) {
            return 1;
        }
    }
    return param_2;
}
