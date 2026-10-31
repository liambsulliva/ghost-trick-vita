void func_020570c4(char *object, int first, int second, int third);
void func_0202c700(int param_1, int param_2, unsigned int param_3);

void func_0205700c(int param_1, int param_2, int param_3, int param_4, int param_5)
{
    char buf_a[8];
    char buf_b[8];
    char buf_c[8];
    char buf_d[8];
    int a;
    int b;
    int c;
    int d;

    a = (short)((((param_2 >> 7) & 0x80000000) >> 16) | ((param_2 >> 7) & 0x7fff));
    b = (short)((((param_3 >> 7) & 0x80000000) >> 16) | ((param_3 >> 7) & 0x7fff));
    c = (short)((((param_4 >> 7) & 0x80000000) >> 16) | ((param_4 >> 7) & 0x7fff));
    d = (short)((((param_5 >> 7) & 0x80000000) >> 16) | ((param_5 >> 7) & 0x7fff));
    func_020570c4(buf_a, a, b, 0);
    func_0202c700(param_1, 0, (unsigned int)buf_a);
    func_020570c4(buf_b, a, d, 0);
    func_0202c700(param_1, 1, (unsigned int)buf_b);
    func_020570c4(buf_c, c, d, 0);
    func_0202c700(param_1, 2, (unsigned int)buf_c);
    func_020570c4(buf_d, c, b, 0);
    func_0202c700(param_1, 3, (unsigned int)buf_d);
}
