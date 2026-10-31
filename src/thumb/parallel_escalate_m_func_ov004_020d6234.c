struct Panel {
    unsigned char pad000[0xe0];
    int we0;
    unsigned char pad0e4[0x9cc - 0xe4];
    int w9cc;
    int w9d0;
    unsigned char pad9d4[0xaa8 - 0x9d4];
    unsigned char aa8_b0:1;
    unsigned char aa8_b1:1;
    unsigned char aa8_mid:4;
    unsigned char aa8_b6:1;
    unsigned char aa8_b7:1;
    unsigned char aa9_b0:1;
    unsigned char aa9_high:7;
    unsigned char mode:3;
};

extern int func_ov004_020d3da4(int a);
extern int func_ov004_020d3e10(int a);
extern void func_ov004_020d6590(struct Panel *p);
extern void func_ov004_020d6704(struct Panel *p);
extern void func_ov004_020d64b4(struct Panel *p);
extern int func_ov004_020d6870(struct Panel *p);
extern int func_ov004_020d69f0(struct Panel *p);

void func_ov004_020d6234(struct Panel *param_1)
{
    do {
        if ((param_1->mode & 1) != 0) {
            if (func_ov004_020d3da4(param_1->w9cc) == 0) {
                return;
            }
        }
        if ((param_1->mode & 2) != 0) {
            if (func_ov004_020d3da4(param_1->w9d0) == 0) {
                return;
            }
        }
        if ((param_1->mode & 1) != 0) {
            func_ov004_020d3e10(param_1->w9cc);
        }
        if ((param_1->mode & 2) != 0) {
            func_ov004_020d3e10(param_1->w9d0);
        }
        if (!param_1->aa8_b1) {
            param_1->aa8_b1 = 1;
        }
        param_1->we0++;
        func_ov004_020d6590(param_1);
        func_ov004_020d6704(param_1);
        if (param_1->aa9_b0) {
            func_ov004_020d64b4(param_1);
        }
        if (param_1->aa8_b6) {
            return;
        }
        if (func_ov004_020d6870(param_1) == 0) {
            return;
        }
    } while (func_ov004_020d69f0(param_1) != 0);
}
