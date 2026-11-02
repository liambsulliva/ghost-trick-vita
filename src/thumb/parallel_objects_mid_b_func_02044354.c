struct Part {
    char pad0[0x28];
    int w28;
    int w2c;
    char pad1[0xa0];
    int wd0;
    int wd4;
    int wd8;
    int wdc;
};

void func_02044354(struct Part *p, int a, int b, int c, int d)
{
    p->wd0 = p->w28 + a;
    p->wd4 = p->w2c + b;
    p->wd8 = p->w28 + c;
    p->wdc = p->w2c + d;
}
