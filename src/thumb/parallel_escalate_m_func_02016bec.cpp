class Obj {
public:
    virtual void v0();
    virtual void v1();
};

extern "C" {
int func_02016a64(Obj *obj);
void *func_02031254(int size);
Obj *func_0201abcc(void *mem);
Obj *func_0201b170(void *mem);

Obj *func_02016bec(char *param_1, int param_2)
{
    Obj *o;

    if (*(Obj **)(param_1 + 0xc) != 0) {
        if (param_2 == func_02016a64(*(Obj **)(param_1 + 0xc))) {
            return *(Obj **)(param_1 + 0xc);
        }
    }
    o = *(Obj **)(param_1 + 0xc);
    if (o != 0) {
        if (o != 0) {
            o->v1();
        }
        *(Obj **)(param_1 + 0xc) = 0;
    }
    switch (param_2) {
    case 0:
        o = (Obj *)func_02031254(0x78);
        if (o != 0) {
            o = func_0201abcc(o);
        }
        *(Obj **)(param_1 + 0xc) = o;
        break;
    case 2:
        o = (Obj *)func_02031254(0x100);
        if (o != 0) {
            o = func_0201b170(o);
        }
        *(Obj **)(param_1 + 0xc) = o;
        break;
    }
    return *(Obj **)(param_1 + 0xc);
}
}
