struct Meter {
    int f00;
    int f04;
    int f08;
    int f0c;
    int f10;
    int f14;
    int f18;
    int f1c;
    int f20;
    short value;
    short h26;
    int f28;
    int f2c;
    unsigned char b30;
    unsigned char b31;
    unsigned char b32;
    unsigned char b33;
    unsigned char b34;
    unsigned char b35;
};

extern void func_02038848();

struct Meter *func_02038150(struct Meter *meter, unsigned char mode, short value)
{
    meter->f00 = 0;
    meter->f04 = 0;
    meter->f08 = 0;
    meter->f0c = 0;
    meter->f10 = 0;
    meter->f14 = 0;
    meter->f18 = 0;
    meter->f1c = -1;
    meter->f20 = 0;
    meter->value = value;
    meter->h26 = 0;
    meter->f28 = 0;
    meter->f2c = 0;
    meter->b30 = 0;
    meter->b31 = 0;
    meter->b32 = 0;
    meter->b33 = 0;
    meter->b34 = 0;
    meter->b35 = 0;
    func_02038848(meter, mode);
    return meter;
}
