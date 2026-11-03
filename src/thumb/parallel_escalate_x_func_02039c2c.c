extern unsigned int *func_02008444(void);
extern int func_0201ff10(unsigned int *p, int z);
extern int func_0201fbc8(int p);
extern int func_02037130(unsigned char v);
extern int func_0201f944(int a, int b);
extern int func_02037170(int v);
extern unsigned char *func_02009900(void);
extern void func_0202f7e8(unsigned char *p, int v, int a, int b);
extern int func_02039ee4(void *s, int idx, int a, int b);
extern void func_02039f3c(void *s, int *arr);

struct View02039c2c {
    int w0;
    int w1;
    int w2;
    int w3;
    unsigned char mode;
    unsigned char flag;
};

struct View02039c2c *func_02039c2c(struct View02039c2c *s, short key, unsigned char flag, unsigned char mode)
{
    int slots[5];
    int r;
    int u;
    int i;
    int end;

    s->w0 = 0;
    s->w1 = 0;
    s->w2 = 0;
    s->w3 = 0;
    s->mode = mode;
    s->flag = flag;
    if (flag == 0) {
        s->w2 = func_0201ff10(func_02008444(), 0);
        if (s->w2 != 0) {
            s->w3 = func_0201fbc8(s->w2);
        }
        i = 0;
        u = key - 2;
        end = key + 3;
        if (u < end) {
            while (u < end) {
                slots[i] = 0;
                if (u >= 0 && (unsigned int)u < (unsigned int)s->w3) {
                    r = func_02037130(u);
                    if (s->mode == 0 || (s->mode != 0 && func_0201f944(s->w2, r) >= 0 && func_02037170(r) == 0)) {
                        slots[i] = func_02039ee4(s, u, (signed char)i, u == key);
                        func_0202f7e8(func_02009900(), slots[i], 7, 0);
                    }
                }
                u++;
                i++;
            }
        }
    } else {
        s->mode = 0;
        u = -2;
        i = 0;
        do {
            slots[i] = 0;
            if ((unsigned int)u <= 1) {
                slots[i] = func_02039ee4(s, u, (signed char)i, u == key);
                func_0202f7e8(func_02009900(), slots[i], 7, 0);
            }
            i++;
            u++;
        } while (i < 5);
    }
    func_02039f3c(s, slots);
    return s;
}
