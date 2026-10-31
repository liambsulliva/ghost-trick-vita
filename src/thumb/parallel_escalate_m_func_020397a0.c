extern void func_02039300(void *s);
extern void func_02039294(void *s, int v);
extern void func_02039324(void *s);
extern void func_020392b0(void *s, int v);

struct Step {
    unsigned char unk_00[0x10];
    unsigned char state10;
    unsigned char unk_11[0xb0 - 0x11];
    int a_target;
    int b_target;
    unsigned char unk_b8[0xc4 - 0xb8];
    int a_cur;
    int b_cur;
    unsigned char unk_cc[0xdf - 0xcc];
    unsigned char f_df;
    unsigned char unk_e0;
    unsigned char f_e1;
};

void func_020397a0(struct Step *s)
{
    int cur;
    int tgt;
    int d;

    cur = s->a_cur;
    tgt = s->a_target;
    if (tgt > cur) {
        d = tgt - cur;
        if (d < 0x1000) {
            func_02039300(s);
        } else if (d < 0x3000) {
            func_02039294(s, -0x1000);
        } else {
            func_02039294(s, -0x2000);
        }
    } else if (tgt < cur) {
        d = cur - tgt;
        if (d < 0x1000) {
            func_02039300(s);
        } else if (d < 0x3000) {
            func_02039294(s, 0x1000);
        } else {
            func_02039294(s, 0x2000);
        }
    }
    cur = s->b_cur;
    tgt = s->b_target;
    if (tgt > cur) {
        d = tgt - cur;
        if (d < 0x1000) {
            func_02039324(s);
        } else if (d < 0x3000) {
            func_020392b0(s, -0x1000);
        } else {
            func_020392b0(s, -0x4000);
        }
    } else if (tgt < cur) {
        d = cur - tgt;
        if (d < 0x1000) {
            func_02039324(s);
        } else if (d < 0x3000) {
            func_020392b0(s, 0x1000);
        } else {
            func_020392b0(s, 0x4000);
        }
    }
    if (s->a_target == s->a_cur && s->b_target == s->b_cur) {
        s->f_df = 0;
        s->f_e1 = 0;
        s->state10 = 0x33;
    }
}
