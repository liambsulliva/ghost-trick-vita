extern void *func_0203ec54(void *);
extern void func_ov006_020d6f80(void *, void *);
extern void func_ov006_020d8174(void *, int, int);
extern void *func_02009900(void);
extern void func_0202f7e8(void *, void *, int, int);
void *func_ov006_020d813c(void *a, int b, int c)
{
    void *item = func_0203ec54((void *)0x020ce17cu);
    func_ov006_020d6f80(item, a);
    func_ov006_020d8174(item, b, c);
    func_0202f7e8(func_02009900(), item, 1, 0);
    return item;
}
