extern void func_0200d4d4();
extern void func_0200d4f8();
extern int func_0200d1ec();
extern void func_0200d1e8();
extern void func_0200d590();
extern unsigned int func_0200d1f0();
extern char data_020b8758;

unsigned short func_0200d2c8(void)
{
    unsigned int c;
    int n;
    int r;
    int g;
    int b;
    int v;

    func_0200d4d4(&data_020b8758);
    func_0200d4f8(&data_020b8758);
    n = func_0200d1ec(&data_020b8758);
    n = (n * 3) >> 2;
    func_0200d1e8(&data_020b8758, n);
    func_0200d590(&data_020b8758);
    c = func_0200d1f0(&data_020b8758);
    r = (c & 0x1f) << 3;
    g = ((int)(c & 0x3e0) >> 5) << 3;
    b = ((int)(c & 0x7c00) >> 10) << 3;
    r = r + r / 4;
    g = g - g / 4;
    if (g < 0) {
        g = 0;
    }
    b = b - b / 4;
    if (b < 0) {
        b = 0;
    }
    v = r >> 3;
    if (v > 0x1f) {
        v = 0x1f;
    }
    return ((g >> 3) << 5) | v | ((b >> 3) << 10);
}
