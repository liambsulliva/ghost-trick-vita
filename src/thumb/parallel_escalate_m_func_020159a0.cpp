class V020159a0 {
public:
    virtual void v00();
    virtual void v04();
    virtual void v08();
    virtual void v0c();
    virtual void v10();
    virtual void v14();
    virtual void v18();
    virtual void v1c();
    virtual void v20();
};

struct Obj020159a0 {
    V020159a0 *a;
    V020159a0 *b;
    int c;
    int d;
    int count;
    char name[0x40];
};

extern "C" {
extern void func_0207c594(void *dst, int size);
extern void *func_02008444(void);
extern char *func_02015a0c(void *object);
extern char *func_02078e84(char *dst, const char *src);
extern int func_020790c8(const char *a, const char *b);
extern void func_0202f64c(void *object);

void func_020159a0(unsigned char *param_1)
{
    char buf[0x40];

    func_0207c594(buf, 0x40);
    func_02078e84(buf, func_02015a0c(func_02008444()));
    if (func_020790c8((char *)(param_1 + 0x14), buf) != 0) {
        func_02078e84((char *)(param_1 + 0x14), buf);
        *(int *)(param_1 + 0x10) = *(int *)(param_1 + 0x10) + 1;
        (*(V020159a0 **)param_1)->v20();
    } else {
        (*(V020159a0 **)param_1)->v1c();
    }
    (*(V020159a0 **)(param_1 + 4))->v1c();
    func_0202f64c(param_1 + 8);
    func_0202f64c(param_1 + 0xc);
}
}
