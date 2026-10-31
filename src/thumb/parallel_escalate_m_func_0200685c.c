struct State {
    unsigned int before[4];
    int data;
};
typedef struct {
    unsigned int flags;
    int mode;
    char pad8[0x20];
    int m28;
    int pad2c;
    int m30;
    int ch;
} Snd0200685c;
extern struct State data_020b18a0;
extern void func_0207680c();
extern void func_02007a8c();
extern unsigned int func_02076ddc();
extern void func_02076b60();
extern void func_02076cfc();
extern void func_02076734();

void func_0200685c(void)
{
    Snd0200685c *s;
    unsigned int mask;
    int t;

    mask = 0;
    s = (Snd0200685c *)(data_020b18a0.data + 0x4c50);
    func_0207680c(0);
    if (s->ch >= 0) {
        mask = 1 << s->ch;
    }
    func_02007a8c(s->m28, s->m30, mask, 0);
    if (s->ch >= 0) {
        mask = func_02076ddc();
        func_02076b60(1);
        func_02076cfc(mask);
    }
    if (s->mode == 1) {
        func_02076734(0, 0, 0, 0);
    }
    t = s->flags;
    t = t & ~1;
    s->flags = t;
}
