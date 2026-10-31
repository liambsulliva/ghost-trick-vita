class Name {
public:
    virtual void v00();
    virtual void v04();
    virtual void v08();
    virtual void v0c();
    virtual void *v10(void);
};

class Scene {
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
    virtual void v24();
    virtual void v28();
    virtual void v2c();
    virtual void v30();
    virtual void v34();
    virtual void v38();
    virtual void v3c();
    virtual void v40();
    virtual void v44();
    virtual int v48(void);
};

extern "C" {

extern void *func_02008444(void);
extern int func_02027ea4(void *a);
extern int func_02027ec8(void *a, void *b);
extern int func_02027eb8(void *a);
extern int func_0200f330(void *a);
extern int func_02011244(void *a);
extern void *func_02011264(void);
extern void *func_0201123c(void *a, int b);
extern Name *func_02022f04(void *a, int b);
extern unsigned char data_02085078[];
extern unsigned char data_02084fe0[];
extern unsigned char data_020850e4[];
extern unsigned char data_020850f8[];

int func_02027db8(Scene *param_1)
{
    Name *name;
    void *text;
    int ret;
    void *list;

    if (func_02027ea4(func_02008444()) != 0) {
        return 1;
    }
    switch (param_1->v48()) {
    case 0:
    default:
        if (func_0200f330(func_02008444()) == 0) {
            return 0;
        }
        return 1;
    case 1:
        break;
    case 2:
        return func_02011244(func_02008444());
    case 3:
        return 0;
    }
    if (func_02011244(func_02008444()) != 0) {
        return 1;
    }
    if (func_02027eb8(func_02008444()) == 0) {
        if (func_0200f330(func_02008444()) == 0) {
            return 0;
        }
        return 1;
    }
    list = func_02011264();
    ret = 0;
    name = func_02022f04(func_0201123c(list, 0), -1);
    if (name != 0) {
        text = name->v10();
        if (func_02027ec8(text, data_02085078) != 0 || func_02027ec8(text, data_02084fe0) != 0 || func_02027ec8(text, data_020850e4) != 0 || func_02027ec8(text, data_020850f8) != 0) {
            return 0;
        }
        return 1;
    }
    return ret;
}
}
