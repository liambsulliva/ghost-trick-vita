class Item {
public:
    virtual void slot0();
    virtual void slot1();
};
extern "C" {
extern void *func_02031254(int);
extern void *func_0200b460(void *);
extern Item *func_0200eaec(Item *);
extern void func_0200b518(void *obj, int a, unsigned int b, unsigned int c);
extern void func_0200b598(void *obj, int a, int b);
extern void func_020280a8(void *obj, int v);
extern int func_020409a8(int v);
extern void func_020416a0(void *obj, int x, int y);
extern void func_0200bbf8(void *, Item *);

void func_020415b0(unsigned char *p, int glyph, int b, int x, int y, unsigned char flag)
{
    void *obj;
    Item *item;
    int dx;
    int dy;

    if (p[0xbc] < 11) {
        obj = func_02031254(0x88);
        if (obj) obj = func_0200b460(obj);
        *(void **)(p + p[0xbc] * 4 + 0xc0) = obj;
        if (*(void **)(p + p[0xbc] * 4 + 0xc0) != 0) {
            func_0200b518(*(void **)(p + p[0xbc] * 4 + 0xc0), 1, 0x80000000, 0x80000000);
            func_0200b598(*(void **)(p + p[0xbc] * 4 + 0xc0), glyph, 15);
            func_020280a8(*(void **)(p + p[0xbc] * 4 + 0xc0), b);
            dx = func_020409a8(*(int *)(p + 0xb0));
            dy = func_020409a8(*(int *)(p + 0xb4));
            func_020416a0(*(void **)(p + p[0xbc] * 4 + 0xc0), x + dx, y + dy);
            if (flag) {
                item = (Item *)func_02031254(4);
                if (item) item = func_0200eaec(item);
                if (item) {
                    func_0200bbf8(*(void **)(p + p[0xbc] * 4 + 0xc0), item);
                    if (item) item->slot1();
                }
            }
            p[0xbc]++;
        }
    }
}
}
