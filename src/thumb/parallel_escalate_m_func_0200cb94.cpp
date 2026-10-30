class Item {
public:
    virtual void slot0();
    virtual void slot1();
    virtual void slot2();
    virtual void slot3();
    virtual void slot4();
    virtual void slot5();
    virtual void slot6();
    int words[5];
};
extern "C" {
struct Owner {
    int word0;
    char list[0x34];
    unsigned int count;
    Item *items;
};
extern int func_0200cbd0(Item *item);
extern int func_0200cbe4(Item *item);
extern void func_020226e8(void *list,int value);
void func_0200cb94(struct Owner *owner)
{
    Item *item;
    unsigned int i;
    item = owner->items;
    i = 0;
    if (i < owner->count) {
        for (; i < owner->count; i++, item++) {
            if (func_0200cbd0(item) != 0) {
                func_020226e8(&owner->list,func_0200cbe4(item));
                item->slot6();
            }
        }
    }
}
}
