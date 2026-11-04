class Heap0201a4ec {
public:
    virtual void slot0();
    virtual void slot1();
    virtual void *alloc(unsigned int size, void *tag);
};

struct Obj0201a4ec {
    char pad0[0x68];
    unsigned char *buf;
    void *bufs[4][2];
    char pad8c[0xc];
    unsigned char head0[0x24];
    unsigned char head1[0x24];
};

extern "C" {
extern void *func_0200883c(void);
extern void *func_0201a4e8(void *object);
extern unsigned char *func_0201e7f8(void *loader, int index, unsigned int id, unsigned char *dst);
extern unsigned char *func_0201e888(void *loader, int index, unsigned int id, int flag, void *arg);
extern unsigned int func_0200cb5c(void *loader);
extern void *func_02007bd8(void);
extern Heap0201a4ec *func_02007b70(void *system);
extern void func_0207303c(const void *src, void *dest, unsigned int size);
extern const unsigned short data_02084a6c[2][4];
extern char data_02084cd8;

void func_0201a4ec(Obj0201a4ec *obj)
{
    unsigned char *src[4];
    unsigned int size0;
    unsigned int size1;
    unsigned int size2;
    unsigned int size3;
    unsigned char i;

    func_0201e7f8(func_0201a4e8(func_0200883c()), 0, 0x6d, obj->buf);
    func_0207303c(obj->buf, obj->head0, 0x24);
    func_0201e7f8(func_0201a4e8(func_0200883c()), 0, 0x73, obj->buf);
    func_0207303c(obj->buf, obj->head1, 0x24);
    for (i = 0; i < 2; i++) {
        src[0] = func_0201e888(func_0201a4e8(func_0200883c()), 0, data_02084a6c[i][0], 1, 0);
        size0 = func_0200cb5c(func_0201a4e8(func_0200883c())) - 0x24;
        src[1] = func_0201e888(func_0201a4e8(func_0200883c()), 0, data_02084a6c[i][1], 1, 0);
        size1 = func_0200cb5c(func_0201a4e8(func_0200883c())) - 0x24;
        src[2] = func_0201e888(func_0201a4e8(func_0200883c()), 0, data_02084a6c[i][2], 1, 0);
        size2 = func_0200cb5c(func_0201a4e8(func_0200883c())) - 0x24;
        src[3] = func_0201e888(func_0201a4e8(func_0200883c()), 0, data_02084a6c[i][3], 1, 0);
        size3 = func_0200cb5c(func_0201a4e8(func_0200883c())) - 0x24;
        obj->bufs[0][i] = func_02007b70(func_02007bd8())->alloc(size0, &data_02084cd8);
        obj->bufs[1][i] = func_02007b70(func_02007bd8())->alloc(size1, &data_02084cd8);
        obj->bufs[2][i] = func_02007b70(func_02007bd8())->alloc(size2, &data_02084cd8);
        obj->bufs[3][i] = func_02007b70(func_02007bd8())->alloc(size3, &data_02084cd8);
        src[0] += 0x24;
        func_0207303c(src[0], obj->bufs[0][i], size0);
        src[1] += 0x24;
        func_0207303c(src[1], obj->bufs[1][i], size1);
        src[2] += 0x24;
        func_0207303c(src[2], obj->bufs[2][i], size2);
        src[3] += 0x24;
        func_0207303c(src[3], obj->bufs[3][i], size3);
    }
}
}
