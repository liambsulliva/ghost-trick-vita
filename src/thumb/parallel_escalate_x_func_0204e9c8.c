struct Span0204e9c8 {
    short start;
    short end;
};

struct Keys0204e9c8 {
    int k[3];
};

extern const struct Span0204e9c8 data_0209bda0[];
extern const struct Keys0204e9c8 data_0209bdac[];
extern const struct Keys0204e9c8 data_0209bdd0[];
extern char data_020a46c8;
extern void func_020082d8(void *out, void *src);
extern void func_02008398(void *out, int x, int y, int z);
extern int func_0204ead0(int a);
extern int func_0204eae0(int a);
extern int func_0206f28c(int a, int b);

void func_0204e9c8(void *out, void *obj, int index, int angle, int time)
{
    int end;
    int start;
    int x;
    int y;
    int s;
    int c;
    int a;
    int b;

    end = data_0209bda0[index].end;
    x = 0;
    y = 0;
    if (time >= end) {
        func_020082d8(out, &data_020a46c8);
    } else {
        start = data_0209bda0[index].start;
        if (time < start) {
            x = data_0209bdac[index].k[0] + time * (data_0209bdac[index].k[1] - data_0209bdac[index].k[0]) / start;
            y = data_0209bdd0[index].k[0] + time * (data_0209bdd0[index].k[1] - data_0209bdd0[index].k[0]) / start;
        } else if (time < end) {
            x = data_0209bdac[index].k[1] + (time - start) * (data_0209bdac[index].k[2] - data_0209bdac[index].k[1]) / (end - start);
            y = data_0209bdd0[index].k[1] + (time - start) * (data_0209bdd0[index].k[2] - data_0209bdd0[index].k[1]) / (end - start);
        }
        s = func_0204ead0(angle * 0x100);
        c = func_0204eae0(angle * 0x100);
        a = func_0206f28c(x, c) - func_0206f28c(y, s);
        b = func_0206f28c(x, s) + func_0206f28c(y, c);
        func_02008398(out, a, b, 0);
    }
}
