typedef int code();

extern const unsigned char data_0209e010[];
extern const unsigned char data_0209e011[];
extern const unsigned char data_0209e048[];
extern unsigned char data_0209e110[];

extern void *func_02007bd8(void);
extern void *func_02007b70(void *object);
extern void func_020649e4(void *object, int value);
extern void *func_0200883c(void);
extern void *func_02008824(void *object);
extern long long func_0201e888(void *object, int a, unsigned int b, int c, int *d);
extern void func_0207303c(void *dst, void *src, int n);

int func_020648b4(unsigned char *p)
{
    int idx;
    int *obj;
    unsigned short *buf;
    unsigned int i;
    int entry;
    unsigned int j;
    int offset;

    idx = *(int *)(p + 0x28) * 2;
    entry = (int)(data_0209e048 + data_0209e010[idx] * 8);
    if (*(int *)(p + 0x38) != 0 && *(int *)(p + 0x3c) != 0) {
        return 1;
    }
    if (*(int *)(p + 0x38) == 0) {
        *(int *)(p + 0x40) = (data_0209e011[idx] + 1) * 0x6000;
        obj = (int *)func_02007b70(func_02007bd8());
        *(int *)(p + 0x38) = (**(code **)(*obj + 8))(obj, *(int *)(p + 0x40), data_0209e110);
    }
    if (*(int *)(p + 0x3c) == 0) {
        *(int *)(p + 0x44) = data_0209e011[*(int *)(p + 0x28) * 2] << 0xb;
        obj = (int *)func_02007b70(func_02007bd8());
        *(int *)(p + 0x3c) = (**(code **)(*obj + 8))(obj, *(int *)(p + 0x44), data_0209e110);
    }
    if (*(int *)(p + 0x38) == 0 || *(int *)(p + 0x3c) == 0) {
        return 0;
    }
    func_020649e4((void *)*(int *)(p + 0x38), *(int *)(p + 0x40));
    for (i = 0; i < data_0209e010[*(int *)(p + 0x28) * 2 + 1]; i++) {
        buf = (unsigned short *)(unsigned int)func_0201e888(func_02008824(func_0200883c()), 2, *(const unsigned int *)entry & 0xffff, 1, (int *)0);
        func_0207303c(buf, (void *)(*(int *)(p + 0x38) + i * 0x6000), 0x6000);
        buf = (unsigned short *)(unsigned int)func_0201e888(func_02008824(func_0200883c()), 2, ((const unsigned int *)entry)[1] & 0xffff, 1, (int *)0);
        if (i != 0) {
            for (j = 0; j < 0x300; j++) {
                buf[j] += i * 0x300;
            }
        }
        offset = (int)(i * 0x600) & ~1;
        func_0207303c(buf, (void *)(*(int *)(p + 0x3c) + offset), 0x800);
        entry = entry + 8;
    }
    return 1;
}
