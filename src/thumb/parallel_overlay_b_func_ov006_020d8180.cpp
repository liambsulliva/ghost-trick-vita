class Item {
public:
    virtual void slot0();
    virtual void slot1();
};
struct Bits { unsigned char first : 1; unsigned char second : 1; unsigned char rest : 6; };
extern "C" {
extern void func_0200b598(void *, int, int);
extern Item *func_02031254(int);
extern Item *func_0200eaec(Item *);
extern void func_0200bbf8(void *, Item *);
extern void func_0200bb64(void *);
extern void func_ov006_020d8220(void *);
void func_ov006_020d8180(unsigned char *p)
{
    if (*(int *)(p + 0xbc) != -1) {
        func_0200b598(p + 0x28, *(int *)(p + 0xbc), 15);
        *(int *)(p + 0xbc) = -1;
        func_ov006_020d8220(p);
    }
    if (((Bits *)(p + 0xc2))->first) {
        Item *item = func_02031254(4);
        if (item) item = func_0200eaec(item);
        if (item) {
            func_0200bbf8(p + 0x28, item);
            if (item) item->slot1();
        }
        ((Bits *)(p + 0xc2))->first = 0;
        func_ov006_020d8220(p);
    }
    if (((Bits *)(p + 0xc2))->second) {
        func_0200bb64(p + 0x28);
        ((Bits *)(p + 0xc2))->second = 0;
        func_ov006_020d8220(p);
    }
}
}
