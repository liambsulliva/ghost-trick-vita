struct Part {
    char pad0[0x28];
    int w28;
    int w2c;
    char pad1[0xa8];
    int wd8;
    int wdc;
    int we0;
    int we4;
};

void func_0203b1b8(struct Part *p, int a, int b, int c, int d)
{
    p->wd8 = p->w28 + a;
    p->wdc = p->w2c + b;
    p->we0 = p->w28 + c;
    p->we4 = p->w2c + d;
}
