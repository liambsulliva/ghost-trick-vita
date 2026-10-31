extern void *func_0200c4e4(void *object);
extern void *func_0200c504(void *object);
extern void *func_0201e2cc(void *object);
extern void func_0201e2ec(void *object, int a, int b, int c, int d);
extern void func_0200c530(void *object, const void *data);
extern void *func_02007bd8(void);
extern void func_0201f288(void *object, void *value);
extern void func_0200c5c4(void *object);
extern const char data_02083ba8[];

char *func_0200c534(char *p)
{
    func_0200c4e4(p);
    func_0200c4e4(p + 0xa88);
    *(int *)(p + 0x1528) = 0;
    func_0200c504(p + 0x152c);
    func_0201e2cc(p + 0x1568);
    func_0201e2ec(p + 0x1568, 0x28, 0x100, 0, 0);
    func_0200c530(p + 0x1568, data_02083ba8);
    func_0201f288(func_02007bd8(), p + 0x1568);
    *(int *)(p + 0x1510) = 0;
    *(int *)(p + 0x1514) = 0;
    *(int *)(p + 0x1518) = 0;
    *(int *)(p + 0x151c) = 0;
    *(int *)(p + 0x1520) = 0;
    *(int *)(p + 0x1524) = 0;
    func_0200c5c4(p);
    return p;
}
