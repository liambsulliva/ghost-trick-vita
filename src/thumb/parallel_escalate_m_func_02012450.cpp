class Item02012450 {
public:
    virtual ~Item02012450();
    virtual void v08();
    virtual void v0c();
    virtual void v10();
    virtual void v14(int owner, Item02012450 *item);
};

class Factory02012450 {
public:
    virtual Item02012450 *create();
};

struct Owner02012450 {
    char pad[0xe5c];
    Item02012450 *slots[32];
    unsigned int count;
};

static inline Item02012450 **Slot02012450(Owner02012450 *owner, unsigned int index)
{
    return &owner->slots[index];
}

extern "C" {
extern char data_02083eec;
extern char data_020bbbb4;
extern char *func_02078e84(char *dst, const char *src);
extern char *func_020790a4(char *dst, const char *src);
extern const char *func_02012500(Item02012450 *item);
extern Factory02012450 *func_0201ee30(const char *name, void *table);

void func_02012450(int param_1)
{
    char buf[0x40];
    unsigned int i;
    Item02012450 *item;
    Factory02012450 *factory;
    Item02012450 *made;

    if (((Owner02012450 *)param_1)->count != 0) {
        for (i = 0; i < ((Owner02012450 *)param_1)->count; i++) {
            item = *Slot02012450((Owner02012450 *)param_1, i);
            func_02078e84(buf, &data_02083eec);
            func_020790a4(buf, func_02012500(item));
            factory = func_0201ee30(buf, &data_020bbbb4);
            if (factory != 0) {
                made = factory->create();
                if (made != 0) {
                    made->v14(param_1, item);
                    delete made;
                }
            }
            if (*Slot02012450((Owner02012450 *)param_1, i) != 0) {
                delete *Slot02012450((Owner02012450 *)param_1, i);
                *Slot02012450((Owner02012450 *)param_1, i) = 0;
            }
        }
        ((Owner02012450 *)param_1)->count = 0;
    }
}
}
