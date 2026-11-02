class Mode {
public:
    virtual void s0();
    virtual void s1();
};

struct Iter {
    int words[4];
};

struct Owner {
    char pad0[0xc];
    unsigned int low : 3;
    unsigned int kind : 4;
    unsigned int rest : 25;
    char pad10[0xdc4];
    int palette;
    char pad_dd8[0x108];
    void *engine;
};

extern "C" {
extern char data_020cb04c[];
extern void *func_02009c1c(void);
extern void func_0202f384(void *a);
extern void *func_02031254(int size);
extern Mode *func_0200ead8(void *object);
extern Mode *func_0200eaec(void *object);
extern Mode *func_0200eb00(void *object);
extern Mode *func_0200eb14(void *object);
extern Mode *func_0200eb28(void *object);
extern Mode *func_0200eb3c(void *object);
extern Mode *func_0200eb50(void *object);
extern Mode *func_0200eb64(void *object);
extern void func_0202f3a4(void *a, int b, int c);
extern void func_0202fbb0(Iter *it);
extern int func_0200eac4(Iter *it);
extern void *func_0200eab4(Iter *it);
extern void *func_0200eab8(Iter *it);
extern void func_0200eabc(Iter *it, int step);
extern int func_02008828(void *object, char *type);
extern void func_0202ff14(void *a, Mode *b);
extern void func_0200a2f8(void *engine, int value);
extern void *func_0200eb78(void);
extern void func_0200ead4(void *a, int b);
void func_0200e930(Owner *owner, int kind);
}

void func_0200e930(Owner *owner, int kind)
{
    Mode *mode;
    Iter it;

    owner->kind = kind & 7;
    switch (owner->kind) {
    default:
    case 0:
        func_0202f384(func_02009c1c());
        kind = 0;
        mode = (Mode *)func_02031254(4);
        if (mode != 0) {
            mode = func_0200ead8(mode);
        }
        break;
    case 1:
        kind = 1;
        mode = (Mode *)func_02031254(4);
        if (mode != 0) {
            mode = func_0200eaec(mode);
        }
        break;
    case 2:
        kind = 2;
        mode = (Mode *)func_02031254(4);
        if (mode != 0) {
            mode = func_0200eb00(mode);
        }
        break;
    case 3:
        kind = 3;
        mode = (Mode *)func_02031254(4);
        if (mode != 0) {
            mode = func_0200eb14(mode);
        }
        break;
    case 4:
        kind = 4;
        mode = (Mode *)func_02031254(4);
        if (mode != 0) {
            mode = func_0200eb28(mode);
        }
        break;
    case 5:
        kind = 5;
        mode = (Mode *)func_02031254(4);
        if (mode != 0) {
            mode = func_0200eb3c(mode);
        }
        break;
    case 6:
        kind = 6;
        mode = (Mode *)func_02031254(4);
        if (mode != 0) {
            mode = func_0200eb50(mode);
        }
        break;
    case 7:
        kind = 7;
        mode = (Mode *)func_02031254(4);
        if (mode != 0) {
            mode = func_0200eb64(mode);
        }
        break;
    }
    if (owner->kind != 0) {
        func_0202f3a4(func_02009c1c(), 1, kind);
        func_0202f3a4(func_02009c1c(), 7, kind);
        func_0202f3a4(func_02009c1c(), 0x10, kind);
        func_0202f3a4(func_02009c1c(), 4, kind);
        func_0202f3a4(func_02009c1c(), 0x11, kind);
    }
    if (mode != 0) {
        func_0202fbb0(&it);
        while (func_0200eac4(&it) != 0) {
            if (func_02008828(func_0200eab4(&it), data_020cb04c) != 0) {
                func_0202ff14(func_0200eab8(&it), mode);
            }
            func_0200eabc(&it, 0);
        }
        if (mode != 0) {
            mode->s1();
        }
    }
    func_0200a2f8(owner->engine, owner->palette);
    func_0200ead4(func_0200eb78(), kind);
}
