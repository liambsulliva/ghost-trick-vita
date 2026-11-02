struct Panel0203fb98 {
    char pad0[0x20];
    void *frame;
    void *badge;
    char pad28[4];
    void *label;
};

extern char data_020cdd3c[];
extern char data_020ce298[];
extern char data_020ce2b4[];
extern char data_020ce2d0[];
extern unsigned short data_0209a79c[];
extern unsigned int data_0209b05c[];
extern char data_020c9e48[];

extern int func_0201572c(void);
extern void func_02008444(void);
extern void func_02011404(void);
extern void *func_02011264(void);
extern void *func_0201123c(void *list, int index);
extern void *func_02022f04(void *name, int value);
extern int func_02008828(void *item, void *data);
extern int func_02037a94(void *item);
extern unsigned int func_02037130(unsigned int kind);
extern int func_0203fd54(struct Panel0203fb98 *panel, unsigned int kind);
extern void *func_0203fad8(void *data);
extern void func_0203fcf0(void *frame, int value);
extern void *func_02009900(void);
extern void func_0202f7e8(void *list, void *object, int a, int b);
extern void *func_0203fb48(void *data);
extern void func_0203fcf8(void *object, unsigned int a, unsigned int b, unsigned int c, unsigned int d);
extern void *func_0203fb20(void *data);
extern int func_020371d4(unsigned int kind);
extern int func_02027d4c(unsigned int id, int mode);
extern void *func_0201003c(void);
extern void func_02015cb0(void *object, int value);

int func_0203fb98(struct Panel0203fb98 *panel)
{
    void *list;
    void *item;
    unsigned int kind;
    int skip;
    unsigned int count;
    int result;

    if (func_0201572c() != 0) {
        return 0;
    }
    func_02008444();
    func_02011404();
    list = func_02011264();
    kind = 0;
    item = func_02022f04(func_0201123c(list, 0), -1);
    if (item != 0) {
        if (func_02008828(item, data_020cdd3c) != 0) {
            kind = func_02037a94(item);
        }
    } else {
        return 0;
    }
    panel->frame = 0;
    panel->badge = 0;
    panel->label = 0;
    kind = func_02037130(kind);
    skip = func_0203fd54(panel, kind);
    panel->frame = func_0203fad8(data_020ce298);
    count = data_0209a79c[kind];
    if (count != 0) {
        if (kind == 0x10 && func_020371d4(kind) > 0) {
            func_0203fcf0(panel->frame, count + 1);
        } else {
            func_0203fcf0(panel->frame, count);
        }
    }
    func_0202f7e8(func_02009900(), panel->frame, 10, 0);
    if (skip == 0) {
        panel->badge = func_0203fb48(data_020ce2b4);
        func_0203fcf8(panel->badge, 0x1b4d, 4, 1, 2);
        func_0202f7e8(func_02009900(), panel->badge, 10, 0);
    }
    panel->label = func_0203fb20(data_020ce2d0);
    func_0203fcf8(panel->label, 0x196c, 3, 1, 2);
    func_0202f7e8(func_02009900(), panel->label, 10, 0);
    if (kind == 0x16 && func_020371d4(kind) > 1) {
        kind = 0x17;
    }
    result = func_02027d4c(data_0209b05c[kind], 2);
    if (result != 0) {
        func_02015cb0(func_0201003c(), result);
    }
    *(unsigned int *)(data_020c9e48 + 0x54) = (*(unsigned int *)(data_020c9e48 + 0x54) & 0xfffffdff) | 0xc0001000;
    return 1;
}
