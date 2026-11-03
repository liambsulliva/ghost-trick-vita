struct Pixels0200e290 {
    char pad0[8];
    unsigned char f8;
    char pad9[3];
    int mode;
};

void func_0200e290(struct Pixels0200e290 *param_1, unsigned char *param_2, unsigned int param_3)
{
    int hi;
    int lo;
    unsigned int i;
    unsigned char b;

    if (param_2 == 0) {
        return;
    }
    if (param_1->mode == 2) {
        for (i = 0; i < param_3; i++) {
            b = *param_2;
            lo = b & 0xf;
            hi = b & 0xf0;
            if (lo != 0) {
                if (param_1->f8 != 0) {
                    lo = (unsigned char)(lo + (unsigned char)(param_1->f8 - 3));
                } else {
                    lo = 0;
                }
            }
            if (hi != 0) {
                if (param_1->f8 != 0) {
                    hi = (unsigned char)(hi + (((unsigned int)(param_1->f8 - 3) << 28) >> 24));
                } else {
                    hi = 0;
                }
            }
            *param_2 = hi | lo;
            param_2 = param_2 + 1;
        }
        return;
    }
    if (param_1->mode == 3) {
        for (i = 0; i < param_3; i++) {
            b = *param_2;
            if (b != 0) {
                b = b + (unsigned char)(param_1->f8 - 3);
            }
            *param_2 = b;
            param_2 = param_2 + 1;
        }
    }
}
