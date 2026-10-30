class Target {
public:
    virtual void v00();
    virtual void v04();
    virtual void v08();
    virtual void v0c();
    virtual void v10();
    virtual void v14();
    virtual void v18(int a, void *b, int c);
};

class Item {
public:
    virtual void v00();
    virtual void v04();
};

struct Self {
    int f0;
    int f4;
    int f8;
    int fc;
    int f10;
    Target *f14;
    int f18;
    int f1c;
    int f20;
};

extern "C" {
extern Item *func_02031254(int size);
extern Item *func_0200a378(Item *item);

void func_0200b0b0(Self *param_1, int param_2)
{
    Item *item;

    param_1->f20 = param_2;
    if (param_1->f8 != -1) {
        item = func_02031254(4);
        if (item != 0) {
            item = func_0200a378(item);
        }
        if (item != 0) {
            param_1->f14->v18(param_1->f8, item, param_1->f20 >> 4);
            if (item != 0) {
                item->v04();
            }
        }
    }
}
}
