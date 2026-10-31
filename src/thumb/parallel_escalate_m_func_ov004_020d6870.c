struct Panel {
    unsigned char pad000[0xe4];
    int we4;
    int we8;
    int wec;
    int wf0;
    unsigned char pad0f4[0x9cc - 0xf4];
    int w9cc;
    int w9d0;
    int w9d4;
    unsigned char pad9d8[0xa6c - 0x9d8];
    int wa6c;
    unsigned char pada70[0xaa8 - 0xa70];
    unsigned char aa8_low:2;
    unsigned char first_b:1;
    unsigned char second_b:1;
    unsigned char aa8_b4:1;
    unsigned char aa8_b5:1;
    unsigned char aa8_high:2;
    unsigned char aa9;
    unsigned char mode:3;
};

extern int func_ov004_020d3dc8(int a);
extern void func_ov004_020d5b38(struct Panel *p);

int func_ov004_020d6870(struct Panel *param_1)
{
    int was;

    if ((param_1->mode & 4) != 0) {
        was = 1;
    } else {
        was = 0;
    }
    param_1->wa6c++;
    if ((param_1->mode & 1) != 0) {
        if (++*(int *)((char *)param_1 + 0xe4) == param_1->wec) {
            if (param_1->first_b) {
                *(int *)((char *)param_1 + 0xe4) = 0;
                func_ov004_020d3dc8(param_1->w9cc);
            } else {
                param_1->mode &= ~1;
                if (param_1->w9cc == param_1->w9d4) {
                    param_1->mode &= ~4;
                }
                if ((param_1->mode & 2) == 0) {
                    return 0;
                }
            }
        }
    }
    if ((param_1->mode & 2) != 0) {
        if (++*(int *)((char *)param_1 + 0xe8) == param_1->wf0) {
            if (param_1->second_b) {
                *(int *)((char *)param_1 + 0xe8) = 0;
                func_ov004_020d3dc8(param_1->w9d0);
            } else {
                param_1->mode &= ~2;
                if (param_1->w9d0 == param_1->w9d4) {
                    param_1->mode &= ~4;
                }
                if ((param_1->mode & 1) == 0) {
                    return 0;
                }
            }
        }
    }
    if (was != 0) {
        if ((param_1->mode & 4) == 0) {
            param_1->aa8_b5 = 0;
            func_ov004_020d5b38(param_1);
        }
    }
    return 1;
}
