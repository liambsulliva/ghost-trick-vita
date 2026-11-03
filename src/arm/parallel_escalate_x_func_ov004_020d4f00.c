struct Seg {
    char *f00;
    char *f04;
    char *f08;
    unsigned int f0c;
    char *f10;
    unsigned int f14;
    char *f18;
    int f1c;
    char *f20;
};

extern struct Seg data_ov004_020e4820;
extern void func_02072f54(const void *src, void *dest, unsigned int size);
extern char data_020d764c[];

char *func_ov004_020d4f00(void)
{
    if (data_ov004_020e4820.f20 == 0) {
        if (data_ov004_020e4820.f0c >= 0x180) {
            data_ov004_020e4820.f20 = data_ov004_020e4820.f10;
            func_02072f54(data_020d764c, data_ov004_020e4820.f10, 0x180);
            data_ov004_020e4820.f10 += 0x180;
            data_ov004_020e4820.f0c -= 0x180;
        } else {
            return data_020d764c;
        }
    }
    return data_ov004_020e4820.f20;
}
