extern int data_020a46c8[];
extern int data_020a46b0[];
extern void func_0201f088(int *dst, int a, int b);
extern void func_0201f0a0(int *dst, int *src);
extern void func_020083b0(int *dst, int *src);
extern int func_0201f0b4(int *a, int *b);
extern int func_0201f0d4(int a, int b);
extern int func_0201f0dc(int a);
extern int func_0206f28c(int a, int b);
extern int func_0206ef28(int a, int b);
extern int func_0206ef48(int a);
extern void func_020608d0(int a, int b, int c);

int func_0206083c(int param_1, int param_2, int param_3)
{
    int dir[3];
    int tmp[3];
    int *d;
    int dot;
    int a;
    int b;
    int len;
    int s;

    d = dir;
    func_0201f088(d, param_2, param_3);
    func_0201f0a0(tmp, d);
    func_020083b0(d, tmp);
    if (func_0201f0b4(d, data_020a46c8) != 0) {
        func_020083b0(d, data_020a46b0);
    }
    dot = func_0201f0d4(param_2, param_3);
    a = func_0201f0dc(param_2);
    b = func_0201f0dc(param_3);
    len = func_0206ef28(dot, func_0206f28c(a, b));
    s = func_0206ef48(0x1000 - func_0206f28c(len, len));
    if (dir[2] < 0) {
        func_020608d0(param_1, -s, len);
    } else {
        func_020608d0(param_1, s, len);
    }
    return 1;
}
