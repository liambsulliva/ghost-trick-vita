extern void *func_0200eaec(void *object);
extern void func_0200eb00(void *object);
extern void func_0200eb14(void *object);
extern void func_0200eb28(void *object);
extern void func_0200eb3c(void *object);
extern void func_0200eb50(void *object);
extern void func_0200eb64(void *object);
extern void func_0200a378(void *object);
extern void func_02022544(void *object);
extern void *func_02022514(void *object, unsigned int size, unsigned int a, unsigned int b);

unsigned short *func_02022a10(unsigned short *object)
{
    unsigned int offset;

    offset = 0xf08;
    func_02022514((unsigned char *)object + offset, 0x80, 0x6000, 0x10);
    func_02022514((unsigned char *)object + (offset + 0x34), 0x200, 0x1a000, 0x10);
    func_02022514((unsigned char *)object + (offset + 0x68), 0x280, 0x10000, 0x10);
    func_0200eaec((unsigned char *)object + (offset + 0x9c));
    func_0200eb00((unsigned char *)object + (offset + 0xa0));
    func_0200eb14((unsigned char *)object + (offset + 0xa4));
    func_0200eb28((unsigned char *)object + (offset + 0xa8));
    func_0200eb3c((unsigned char *)object + (offset + 0xac));
    func_0200eb50((unsigned char *)object + (offset + 0xb0));
    func_0200eb64((unsigned char *)object + (offset + 0xb4));
    func_0200a378((unsigned char *)object + (offset + 0xb8));
    *object = 0;
    func_02022544((unsigned char *)object + offset);
    func_02022544((unsigned char *)object + (offset + 0x34));
    func_02022544((unsigned char *)object + (offset + 0x68));
    return object;
}
