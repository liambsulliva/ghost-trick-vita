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
extern char data_020ddafc[];

char *func_ov004_020d4da4(void)
{
    if (data_ov004_020e4820.f08 == 0) {
        if (data_ov004_020e4820.f14 >= 0x659c) {
            data_ov004_020e4820.f08 = data_ov004_020e4820.f18;
            func_02072f54(data_020ddafc, data_ov004_020e4820.f18, 0x659c);
            data_ov004_020e4820.f18 += 0x659c;
            data_ov004_020e4820.f14 -= 0x659c;
        } else {
            return data_020ddafc;
        }
    }
    return data_ov004_020e4820.f08;
}
