class Item {
public:
    virtual void slot0();
    virtual void slot1();
};
extern "C" {
extern void *func_02008444(void);
extern int func_020322d4(void *);
extern unsigned int func_02039710(void);
extern Item *func_02031254(int);
extern Item *func_0200eaec(Item *);
extern Item *func_0200ead8(Item *);
extern void func_0200bbf8(void *, Item *);
void func_020396ac(unsigned char *p, int mode)
{
    Item *item;
    if (func_020322d4(func_02008444()) != 0) return;
    if (p[0xe3] == 0 && func_02039710() == 0) return;
    if (mode != 0) {
        item = func_02031254(4);
        if (item) item = func_0200eaec(item);
    } else {
        item = func_02031254(4);
        if (item) item = func_0200ead8(item);
    }
    if (item) {
        func_0200bbf8(p + 0x28, item);
        if (item) item->slot1();
    }
}
}
