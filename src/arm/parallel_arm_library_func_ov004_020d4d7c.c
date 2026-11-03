struct Seg {
    int f00;
    int f04;
    int f08;
    int f0c;
    int f10;
    int f14;
    int f18;
    int f1c;
    int f20;
};

extern struct Seg data_ov004_020e4820;

void func_ov004_020d4d7c(unsigned int a, unsigned int b)
{
    data_ov004_020e4820.f10 = a;
    data_ov004_020e4820.f0c = b & ~3;
    data_ov004_020e4820.f04 = 0;
    data_ov004_020e4820.f00 = 0;
    data_ov004_020e4820.f20 = 0;
}
