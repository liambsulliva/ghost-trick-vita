struct Key0200cc5c {
    unsigned int index : 10;
    unsigned int id : 16;
    unsigned int rest : 6;
};

struct Flags0200cc5c {
    unsigned int low : 24;
    unsigned int mid : 7;
    unsigned int top : 1;
};

struct Ent0200cc5c {
    int pad0;
    unsigned int value;
    Flags0200cc5c flags;
    int active;
    Key0200cc5c key;
    unsigned int scaled;
};

class Obj0200cc5c {
public:
    virtual void v00();
    virtual void v04();
    virtual void v08();
    virtual void v0c();
    virtual void v10();
    virtual void v14();
    virtual void v18();
    virtual void v1c();
    virtual void v20(int a, unsigned int value, unsigned int b, int c);
    int sub4;
    char pad8[0x30];
    unsigned int count;
    Ent0200cc5c *entries;
    int m40;
    unsigned int m44;
};

extern "C" {
extern void func_0200cb94(Obj0200cc5c *object);
extern unsigned int func_020225e0(int *object, unsigned int key, int arg);

int func_0200cc5c(Obj0200cc5c *param_1, int param_2, unsigned int param_3, unsigned int param_4, unsigned int param_5)
{
    Ent0200cc5c *e;
    unsigned int i;
    int result;

    result = -1;
    func_0200cb94(param_1);
    if (param_5 == 0x80000000) {
        param_5 = func_020225e0(&param_1->sub4, param_3, 0);
    }
    if (param_5 != 0xffffffff) {
        e = param_1->entries;
        for (i = 0; i < param_1->count; i++, e++) {
            if (*(int *)&e->key == 0) {
                *(unsigned int *)&e->key = 0;
                e->key.id = param_4;
                e->key.index = i;
                e->value = param_5;
                e->scaled = param_5 / param_1->m44;
                e->active = 1;
                e->flags.low = param_3;
                e->flags.top = 0;
                result = i;
                param_1->v20(param_2, param_5, param_3, param_1->m40);
                break;
            }
        }
    }
    return result;
}
}
