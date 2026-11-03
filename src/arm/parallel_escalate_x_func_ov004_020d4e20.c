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
extern char data_020db9fc[];

char *func_ov004_020d4e20(void)
{
    if (data_ov004_020e4820.f04 == 0) {
        if (data_ov004_020e4820.f0c >= 0x2100) {
            data_ov004_020e4820.f04 = data_ov004_020e4820.f10;
            func_02072f54(data_020db9fc, data_ov004_020e4820.f10, 0x2100);
            data_ov004_020e4820.f10 += 0x2100;
            data_ov004_020e4820.f0c -= 0x2100;
        } else {
            return data_020db9fc;
        }
    }
    return data_ov004_020e4820.f04;
}
