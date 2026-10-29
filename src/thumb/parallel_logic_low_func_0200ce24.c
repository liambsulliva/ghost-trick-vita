extern void func_0200ce54(int a,int b,int c);
extern void *func_02009c1c(void);
extern void func_0202f1bc(void *table,int a,int b,int c,int d);

void func_0200ce24(int object,int a,int b,int c,int d)
{
    int one;
    int saved_b;
    int base;

    one = 1;
    saved_b = b;
    base = object + 0x48;
    func_0200ce54(a,base + (saved_b & ~one),c);
    func_0202f1bc(func_02009c1c(),a,saved_b,c,d);
}
