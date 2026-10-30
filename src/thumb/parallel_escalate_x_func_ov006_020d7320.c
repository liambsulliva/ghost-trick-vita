extern unsigned int func_ov006_020d7470(int);
extern int func_ov006_020d7fc0(int, unsigned char);
extern unsigned int func_ov006_020d748c(int);
extern void func_ov006_020d7fe8(int);
extern int data_020c9d50[];

signed char func_ov006_020d7320(unsigned char *param_1, int param_2, signed char param_3, signed char param_4, int param_5, int param_6)
{
    signed char i;
    unsigned char f;
    unsigned char z;
    unsigned char y;
    unsigned char x;
    unsigned char c;

    i = param_3;
    y = 0;
    x = 0;
    f = 0;
    z = 0;
    if (param_2 != 0) {
        if (data_020c9d50[3] & 0x10) {
            x = 1;
        }
        if (data_020c9d50[3] & 0x20) {
            y = 1;
        }
        if (data_020c9d50[0] & 0x10) {
            z = 1;
        }
        if (data_020c9d50[0] & 0x20) {
            f = 1;
        }
    } else {
        if (data_020c9d50[3] & 0x400) {
            x = 1;
        }
        if (data_020c9d50[3] & 0x800) {
            y = 1;
        }
        if (data_020c9d50[0] & 0x400) {
            z = 1;
        }
        if (data_020c9d50[0] & 0x800) {
            f = 1;
        }
    }
    if (x) {
        i--;
    } else if (y && !z) {
        i++;
    } else if (param_5 && func_ov006_020d7470(param_5) && !f) {
        i--;
    } else if (param_6 && func_ov006_020d7470(param_6)) {
        if (!(param_5 && func_ov006_020d7470(param_5)) && !z) {
            i++;
        }
    }
    if (i > param_4 - 1) {
        i = param_4 - 1;
    }
    if (i < 0) {
        i = 0;
    }
    c = 0;
    if (i < param_3) {
        if (i == 0) {
            c = 1;
        }
        if (param_5 != 0) {
            func_ov006_020d7fc0(param_5, c);
        }
    }
    if (i > param_3) {
        if (i == param_4 - 1) {
            c = 1;
        }
        if (param_6 != 0) {
            func_ov006_020d7fc0(param_6, c);
        }
    }
    if (i > 0) {
        if (param_5 != 0 && func_ov006_020d748c(param_5)) {
            func_ov006_020d7fe8(param_5);
        }
    }
    if (i < param_4 - 1) {
        if (param_6 != 0 && func_ov006_020d748c(param_6)) {
            func_ov006_020d7fe8(param_6);
        }
    }
    return i - param_3;
}
