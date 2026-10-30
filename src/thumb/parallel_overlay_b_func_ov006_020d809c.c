extern void *func_0203ec40(void *);
extern void func_ov006_020d8000(void *, void *, int, int);
extern void func_ov006_020d8014(void *, int, int, int, int);
extern void *func_02009900(void);
extern void func_0202f7e8(void *, void *, int, int);
void *func_ov006_020d809c(void *a, int b, int c, int d, short e, short f, short g)
{
    void *item = func_0203ec40((void *)0x020ce1b4u);
    func_ov006_020d8000(item, a, b, c);
    func_ov006_020d8014(item, d, e, f, g);
    func_0202f7e8(func_02009900(), item, 1, 0);
    return item;
}
