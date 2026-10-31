class Task {
public:
    virtual void v00();
    virtual void v04();
    virtual void v08();
    virtual void v0c();
    virtual void v10();
    virtual void v14();
    virtual void v18();
    virtual void v1c();
    virtual void v20(int arg);
    char pad04[0xc];
    unsigned char state;
    char pad11[0x33];
    void *x44;
    void *x48;
    void *x4c;
};

extern "C" {
void func_020542b0(void *object);
void func_02055e6c(void *object);
void *func_0201003c(void);
void func_02015970(void *object);
void func_02015990(void *object, int value);
unsigned int func_0202f64c(void *object);
void *func_02009900(void);
unsigned int func_0202fa8c(void *object, void *name);
void *func_02008444(void);
void func_02015288(void *object);
extern char data_020cf438[];
extern char data_020cf588[];

void func_02015204(Task *p)
{
    switch (p->state) {
    case 0:
        if (p->x44 != 0) {
            func_020542b0(p->x44);
            p->x44 = 0;
        }
        if (p->x48 != 0) {
            func_02055e6c(p->x48);
            p->x48 = 0;
        }
        func_02015970(func_0201003c());
        func_02015990(func_0201003c(), 0);
        func_0202f64c(&p->x4c);
        p->state = 1;
        break;
    case 1:
        if (func_0202fa8c(func_02009900(), data_020cf438) == 0) {
            if (func_0202fa8c(func_02009900(), data_020cf588) == 0) {
                func_02015288(func_02008444());
                p->v20(0);
            }
        }
        break;
    }
}
}
