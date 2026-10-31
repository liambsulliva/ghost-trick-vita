class Deletable {
public:
    virtual ~Deletable();
};
class Manager {
public:
    virtual void v00();
    virtual void v04();
    virtual void v08();
    virtual void v0c();
    virtual void v10(void *object);
};
struct Owner {
    unsigned char pad000[0xdc];
    Deletable *child;
    unsigned char pad0e0[0x2b];
    unsigned char b10b;
    unsigned short h10c;
    short h10e;
    int w110;
    void *w114;
};
extern "C" {
extern void *func_02007bd8(void);
extern Manager *func_02007b70(void *object);
extern void *func_0200883c(void);
extern void *func_0201a4e8(void *object);
extern void *func_0201e888(void *object, int a, int b, int c, void *d);

void func_02045650(struct Owner *param_1, unsigned short param_2, unsigned char param_3, unsigned short param_4, short param_5, int param_6)
{
    void *list;

    if (param_1->child != 0) {
        delete param_1->child;
        param_1->child = 0;
    }
    if (param_1->w114 != 0) {
        if (func_02007b70(func_02007bd8()) != 0) {
            func_02007b70(func_02007bd8())->v10(param_1->w114);
            param_1->w114 = 0;
        }
    }
    param_1->b10b = param_3;
    param_1->h10c = param_4;
    param_1->h10e = param_5;
    param_1->w110 = param_6;
    list = func_0201a4e8(func_0200883c());
    param_1->w114 = func_0201e888(list, 0, param_2, 0, func_02007b70(func_02007bd8()));
}
}
