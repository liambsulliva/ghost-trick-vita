struct Panel {
    unsigned char pad000[0xe4];
    int we4;
    int we8;
    int wec;
    int wf0;
    unsigned char pad0f4[0x9cc - 0xf4];
    int w9cc;
    int w9d0;
    unsigned char pad9d4[0xa78 - 0x9d4];
    unsigned char a78[0x14];
    unsigned char a8c[0x1c];
    unsigned char aa8_low:2;
    unsigned char first_b:1;
    unsigned char second_b:1;
    unsigned char aa8_high:4;
    unsigned char aa9_low:5;
    unsigned char first_a:1;
    unsigned char second_a:1;
    unsigned char aa9_high:1;
    unsigned char mode:3;
};

extern int func_ov004_020d6ab4(struct Panel *p, int a, unsigned char *b, int c, int d, int e, int f);
extern void func_ov004_020d5958(struct Panel *p);

int func_ov004_020d69f0(struct Panel *param_1)
{
    int result;

    if ((param_1->mode & 1) != 0) {
        result = func_ov004_020d6ab4(param_1, param_1->w9cc, param_1->a78, param_1->first_a, param_1->first_b, param_1->we4, param_1->wec);
        if (result >= 0) {
            param_1->we4 = result;
        } else if (result == -2) {
            func_ov004_020d5958(param_1);
            return 0;
        }
    }
    if ((param_1->mode & 2) != 0) {
        result = func_ov004_020d6ab4(param_1, param_1->w9d0, param_1->a8c, param_1->second_a, param_1->second_b, param_1->we8, param_1->wf0);
        if (result >= 0) {
            param_1->we8 = result;
        } else if (result == -2) {
            func_ov004_020d5958(param_1);
            return 0;
        }
    }
    return 1;
}
