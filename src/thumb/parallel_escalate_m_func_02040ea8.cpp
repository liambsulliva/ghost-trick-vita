class Item {
public:
    virtual void slot0();
    virtual void slot1();
};
extern "C" {
extern Item *func_02031254(int);
extern Item *func_0200ead8(Item *);
extern void func_0200bbf8(void *, Item *);
void func_02040ea8(unsigned char *p)
{
    Item *item = func_02031254(4);
    if (item) item = func_0200ead8(item);
    if (item) {
        func_0200bbf8(p + 0x28, item);
        if (item) item->slot1();
    }
}
}
