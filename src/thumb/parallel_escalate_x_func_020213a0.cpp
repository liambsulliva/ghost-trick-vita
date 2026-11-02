class Object {
public:
    virtual void slot0();
    virtual void slot1();
    virtual void slot2();
    virtual void slot3();
    virtual void *slot4();
};

struct Info {
    unsigned int type : 8;
    unsigned int flags : 8;
    unsigned int size : 15;
    unsigned int top : 1;
};

struct Desc {
    int word0;
    struct Info info;
    int offset;
};

extern "C" {
extern unsigned int func_02021200(int reader);
extern unsigned long long func_02021244(int reader);
extern unsigned short func_02021288(int reader);
extern long long func_020212d0(int reader);
extern int func_02021314(int reader);
extern short func_02021358(int reader);
extern unsigned char func_0201ea40(int reader);
extern signed char func_0201e9ec(int reader);
extern void func_0201ea1c(int reader, void *dst, unsigned int size);
extern unsigned int func_02020be4(void *klass);
extern void *func_02021658(int ctx, int reader, void *object);
void func_020213a0(int ctx, int reader, char *base, struct Desc *desc);
}

void func_020213a0(int ctx, int reader, char *base, struct Desc *desc)
{
    char *dst;
    unsigned int count;
    unsigned int i;
    unsigned int size;

    dst = base + desc->offset;
    count = 1;
    if (desc->info.flags & 0x20) {
        count = func_02021200(reader);
    }
    switch (desc->info.type) {
    case 1:
        size = func_02020be4(((Object *)dst)->slot4());
        for (i = 0; i < count; i++) {
            func_02021658(ctx, reader, dst + size * i);
        }
        break;
    case 2:
        for (i = 0; i < count; i++) {
            ((void **)dst)[i] = func_02021658(ctx, reader, ((void **)dst)[i]);
        }
        break;
    case 3:
        for (i = 0; i < count; i++) {
            ((unsigned char *)dst)[i] = func_0201ea40(reader) != 0;
        }
        break;
    case 7:
        for (i = 0; i < count; i++) {
            ((unsigned long long *)dst)[i] = func_02021244(reader);
        }
        break;
    case 6:
        for (i = 0; i < count; i++) {
            ((unsigned int *)dst)[i] = func_02021200(reader);
        }
        break;
    case 5:
        for (i = 0; i < count; i++) {
            ((unsigned short *)dst)[i] = func_02021288(reader);
        }
        break;
    case 4:
        for (i = 0; i < count; i++) {
            ((unsigned char *)dst)[i] = func_0201ea40(reader);
        }
        break;
    case 11:
        for (i = 0; i < count; i++) {
            ((long long *)dst)[i] = func_020212d0(reader);
        }
        break;
    case 10:
        for (i = 0; i < count; i++) {
            ((int *)dst)[i] = func_02021314(reader);
        }
        break;
    case 9:
        for (i = 0; i < count; i++) {
            ((short *)dst)[i] = func_02021358(reader);
        }
        break;
    case 8:
        for (i = 0; i < count; i++) {
            ((signed char *)dst)[i] = func_0201e9ec(reader);
        }
        break;
    default:
        func_0201ea1c(reader, dst, desc->info.size * count);
        break;
    }
}
