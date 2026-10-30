class Actor {
public:
    virtual void slot0();
    virtual void slot1();
    virtual void slot2();
    virtual void slot3();
    virtual void slot4();
    virtual void slot5();
    virtual void slot6();
    virtual void slot7();
    virtual void slot8();
    virtual void slot9();
    virtual void reset();
    virtual void slot11();
    virtual void slot12();
    virtual void slot13();
    virtual void slot14();
    virtual void slot15();
    virtual void slot16();
    virtual void slot17();
    virtual void slot18();
    virtual void slot19();
    virtual void slot20();
    virtual void slot21();
    virtual void slot22();
    virtual void slot23();
    virtual void slot24();
    virtual int dead();
};

struct Info {
    unsigned short kind;
    unsigned short x;
};
struct Owner {
    unsigned char pad[0x14];
    Info *info;
    unsigned char pad2[0x20 - 0x18];
    unsigned short mode;
    unsigned char pad3[0x5c - 0x22];
    unsigned char shown;
    unsigned char pad4[0x80 - 0x5d];
    void *list;
};
extern "C" {
extern char data_020b9d84[];
void *func_02031254(int size);
void *func_0201bce4(void *object);
void *func_02009900(void);
void *func_0202fb48(void *manager, char *name, int start);
void func_020183d0(void *object, void *list);
Actor *func_0201bf24(void *list, int x, int flag);
void func_02018710(Actor *actor);
void func_0201c248(Actor *actor, int x, int y);

int func_02018684(Owner *owner)
{
    void *list;
    void *found;
    Actor *actor;
    short x;
    short y;
    if (owner->list == 0) {
        list = func_02031254(0x14);
        if (list != 0) {
            list = func_0201bce4(list);
        }
        owner->list = list;
        found = func_0202fb48(func_02009900(), data_020b9d84, -1);
        if (found != 0) {
            func_020183d0(found, owner->list);
        }
    }
    x = 0;
    actor = func_0201bf24(owner->list, owner->info->x, 0);
    if (actor != 0) {
        func_02018710(actor);
        y = 0x10;
        if (owner->mode == 4) {
            x += 0x18;
            y -= 0x90;
        }
        actor->reset();
        func_0201c248(actor, x << 12, y << 12);
        owner->shown = 1;
    }
    return 0;
}
}
