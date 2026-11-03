extern int func_02009c1c();
extern int func_0200a120();
extern int func_0200a1ac();
extern unsigned char func_0200a668();
extern int func_0202f1bc();

void func_0200a088(int param_1, unsigned int param_2, unsigned int param_3, int param_4, int param_5)
{
    unsigned char c;
    unsigned int y;
    unsigned int x;
    unsigned char *row;
    int *dst;
    int off;

    for (y = param_3; y < param_3 + param_5; y++) {
        row = (unsigned char *)param_1 + 0x6202 + y * 0x40;
        for (x = param_2; x < param_2 + param_4; x++) {
            if (row[x] == 0) {
                row[x] = func_0200a668(param_1 + 0x100);
                c = row[x];
                if (c == 0) {
                    return;
                }
                func_0200a120(param_1, x, y, c);
                func_0200a1ac(param_1, x, y, c);
            }
        }
    }
    off = param_3 * 0x200;
    dst = (int *)func_02009c1c();
    func_0202f1bc(dst, param_1 + 0x202 + off, off, param_5 << 9, 0xf);
}
