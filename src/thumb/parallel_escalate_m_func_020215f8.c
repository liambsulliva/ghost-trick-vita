struct Info {
    unsigned int type : 8;
    unsigned int flags : 8;
    unsigned int count : 15;
    unsigned int top : 1;
};

struct Desc {
    int word0;
    struct Info info;
};

union Value {
    unsigned int value;
    struct {
        unsigned int bit0 : 1;
    } bits;
};

extern unsigned int func_02021200(int reader);
extern void func_0201ea28(int reader, unsigned int count);

void func_020215f8(int param_1, int param_2, struct Desc *param_3)
{
    unsigned int n;
    unsigned int i;

    n = 1;
    if (param_3->info.flags & 0x20) {
        n = func_02021200(param_2);
    }
    switch (param_3->info.type) {
    case 1:
    case 2:
        for (i = 0; i < n; i++) {
            union Value v;
            v.value = func_02021200(param_2);
            if (v.bits.bit0) {
                func_0201ea28(param_2, func_02021200(param_2));
            }
        }
        return;
    }
    func_0201ea28(param_2, param_3->info.count * n);
}
