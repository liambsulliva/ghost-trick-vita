class Item {
public:
    virtual void slot0();
    virtual void slot1();
};
class Manager {
public:
    virtual void v00();
    virtual void v04();
    virtual void v08();
    virtual void *v0c(int a);
};
struct Request {
    void *w0;
    int w4;
    int w8;
    int wc;
};
extern "C" {
extern Item *func_02031254(int);
extern Item *func_0200eaec(Item *);
extern Item *func_0200eb00(Item *);
extern Item *func_0200eb14(Item *);
extern Item *func_0200eb28(Item *);
extern Item *func_0200eb3c(Item *);
extern Item *func_0200eb50(Item *);
extern Item *func_0200eb64(Item *);
extern Item *func_0202f4b0(Item *);
extern void *func_02007bd8(void);
extern Manager *func_02007b70(void *object);
extern void func_0207487c(void *a, int b);
extern void func_0200d114(Item *a, void *b, void *c, int d, int e);

void func_0202f3b0(unsigned char *param_1, struct Request *param_2)
{
    Item *item;
    void *data;
    unsigned char kind;

    kind = (param_1 + param_2->wc)[0x4004];
    if (param_2->w4 == 0 && param_2->wc == 4) {
        return;
    }
    switch (kind) {
    default:
        return;
    case 0:
        return;
    case 1:
        item = func_02031254(4);
        item = item ? func_0200eaec(item) : item;
        break;
    case 2:
        item = func_02031254(4);
        item = item ? func_0200eb00(item) : item;
        break;
    case 3:
        item = func_02031254(4);
        item = item ? func_0200eb14(item) : item;
        break;
    case 4:
        item = func_02031254(4);
        item = item ? func_0200eb28(item) : item;
        break;
    case 5:
        item = func_02031254(4);
        item = item ? func_0200eb3c(item) : item;
        break;
    case 6:
        item = func_02031254(4);
        item = item ? func_0200eb50(item) : item;
        break;
    case 7:
        item = func_02031254(4);
        item = item ? func_0200eb64(item) : item;
        break;
    case 8:
        item = func_02031254(4);
        item = item ? func_0202f4b0(item) : item;
        break;
    }
    if (item) {
        data = func_02007b70(func_02007bd8())->v0c(param_2->w8);
        if (data) {
            func_0207487c(param_2->w0, param_2->w8);
            func_0200d114(item, param_2->w0, data, param_2->w8, 0);
            param_2->w0 = data;
        }
        if (item) item->slot1();
    }
}
}
