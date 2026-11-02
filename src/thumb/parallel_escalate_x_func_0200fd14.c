typedef struct Stage0fd14 {
    char pad0[8];
    unsigned int low8 : 9;
    unsigned int held : 1;
    unsigned int was_held : 1;
    unsigned int high8 : 21;
    unsigned int lowc : 7;
    unsigned int open : 1;
    unsigned int close : 1;
    unsigned int side : 1;
    unsigned int midc : 16;
    unsigned int refresh : 1;
    unsigned int highc : 5;
    char pad10[0xa];
    unsigned char timer;
    char pad1b[0xde1];
    void *items[8];
    char pade1c[0x198];
    void *panel;
    void *effect;
} Stage0fd14;

extern char data_020ce674[];
extern char data_020ce784[];
extern void func_0200fe38(Stage0fd14 *p, int a);
extern int func_02008828(void *a, void *b);
extern int func_0200fe48(void *object);
extern void func_0204fe18(void *object);
extern void *func_02046600(void *a);
extern void *func_02009900(void);
extern void func_0202f7e8(void *a, void *b, int c, int d);
extern void func_02046b1c(void *object);
extern void func_02046b10(void *object);
extern void func_02046b68(void *object);
extern void func_02046b44(void *object);
extern int func_02008860(void);
extern void func_0200c86c(int a, int b);

void func_0200fd14(Stage0fd14 *p)
{
    unsigned int i;
    void *obj;

    func_0200fe38(p, 1);
    p->was_held = p->held;
    p->held = 0;
    if (p->timer != 0) {
        p->timer--;
    }
    for (i = 0; i < 8; i++) {
        if (p->items[i] != 0 && func_02008828(p->items[i], data_020ce674) != 0 && func_0200fe48(p->items[i]) != 0) {
            p->items[i] = 0;
        }
    }
    if (p->effect != 0 && func_0200fe48(p->effect) == 0) {
        func_0204fe18(p->effect);
    }
    if (p->open) {
        p->open = 0;
        if (p->panel == 0) {
            obj = func_02046600(data_020ce784);
            p->panel = obj;
            if (obj != 0) {
                func_0202f7e8(func_02009900(), obj, 1, 0);
            }
        }
        if (p->panel != 0) {
            if (p->side) {
                func_02046b1c(p->panel);
            } else {
                func_02046b10(p->panel);
            }
        }
    }
    if (p->close) {
        p->close = 0;
        if (p->panel != 0) {
            if (p->side) {
                func_02046b68(p->panel);
            } else {
                func_02046b44(p->panel);
            }
        }
    }
    if (p->refresh) {
        p->refresh = 0;
        func_0200c86c(func_02008860(), 0);
        func_0200c86c(func_02008860(), 1);
    }
}
