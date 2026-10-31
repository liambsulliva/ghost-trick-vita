typedef struct {
    char pad[0x1c];
    int a0;
    int b0;
    int inv0;
    int a1;
    int b1;
    int inv1;
    unsigned short enable;
} DivState;

extern DivState data_020d2e84;
extern unsigned int func_02075c98(void);
extern unsigned int func_02075cac(unsigned int bits);

static inline void set_div32(unsigned int numer, unsigned int denom)
{
    *(volatile unsigned short *)0x04000280 = 0;
    *(volatile unsigned int *)0x04000290 = numer;
    *(volatile unsigned long long *)0x04000298 = denom;
}

static inline void wait_div(void)
{
    while (*(volatile unsigned short *)0x04000280 & 0x8000) {
    }
}

static inline int get_div32(void)
{
    return *(volatile int *)0x040002a0;
}

void func_02077ad0(const short *param)
{
    unsigned int bak;
    int d;

    if (param == 0) {
        data_020d2e84.enable = 0;
        return;
    }
    bak = func_02075c98();
    d = param[2];
    if (d != 0) {
        set_div32(0x10000000, d);
        data_020d2e84.a0 = param[0];
        data_020d2e84.b0 = d;
        wait_div();
        data_020d2e84.inv0 = get_div32();
    } else {
        data_020d2e84.a0 = 0;
        data_020d2e84.b0 = 0;
        data_020d2e84.inv0 = 0;
    }
    d = param[3];
    if (d != 0) {
        set_div32(0x10000000, d);
        data_020d2e84.a1 = param[1];
        data_020d2e84.b1 = d;
        wait_div();
        data_020d2e84.inv1 = get_div32();
    } else {
        data_020d2e84.a1 = 0;
        data_020d2e84.b1 = 0;
        data_020d2e84.inv1 = 0;
    }
    func_02075cac(bak);
    data_020d2e84.enable = 1;
}
