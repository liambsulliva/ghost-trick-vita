typedef void code(void);

extern char data_020b1890[];
extern char data_020b1898[];

extern unsigned int func_0206c868(unsigned int *p);
extern int func_02075d50(void);
extern int func_0206cacc(unsigned int a, unsigned int b, unsigned int c, int d);
extern void func_02072fa8(unsigned int a, int b, unsigned int c);
extern void func_02075f84(void);
extern void func_02000950(unsigned int a);
extern void func_0207487c(unsigned int a, unsigned int b);

void func_0206cb8c(unsigned int *p_ovi)
{
    unsigned int size = func_0206c868(p_ovi);

    if (func_02075d50() != 1) {
        int ret = 0;
        if (((p_ovi[7] >> 24) & 2) != 0) {
            int table_max = (data_020b1898 - (data_020b1890 + 8)) / 20;
            if (p_ovi[0] < (unsigned int)table_max) {
                ret = func_0206cacc((unsigned int)(data_020b1890 + 8) + p_ovi[0] * 20, p_ovi[1], size, 0);
            }
        }
        if (ret == 0) {
            func_02072fa8(p_ovi[1], 0, size);
            func_02075f84();
            return;
        }
    }
    if (((p_ovi[7] >> 24) & 1) != 0) {
        func_02000950(p_ovi[1] + size);
    }
    func_0207487c(p_ovi[1], p_ovi[2]);
    {
        code **p = (code **)p_ovi[4];
        code **q = (code **)p_ovi[5];
        for (; p < q; ++p) {
            if (*p) {
                (**p)();
            }
        }
    }
}
