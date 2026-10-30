class Item {
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
    virtual void slot10();
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
    virtual void slot25();
    virtual void slot26();
    virtual void slot27();
    virtual void slot28();
    virtual void slot29();
    virtual void slot30();
    virtual void slot31();
    virtual void slot32();
    virtual void slot33();
    virtual void slot34();
    virtual void slot35();
    virtual void slot36();
    virtual void slot37();
    virtual void slot38();
    virtual void slot39();
    virtual void slot40();
    virtual void slot41();
    virtual void slot42();
    virtual void slot43();
    virtual void slot44();
    virtual void slot45();
    virtual void slot46();
    virtual void slot47();
    virtual void slot48();
    virtual void slot49();
    virtual void slot50();
    virtual void slot51();
    virtual void slot52();
    virtual void slot53();
    virtual int slot54();
};
struct State {
    unsigned char padding[0x158];
    Item *items[2];
    unsigned char more[8];
    unsigned char flag;
    unsigned char more2[11];
    unsigned int counter;
    int current;
    int next;
    int count;
};
extern "C" {
extern void func_0203bfb4(Item *, int);
extern void *func_02010b7c();
extern int func_0202cb64(void *);
extern void func_ov003_020d4ce4(State *, int);
extern void func_ov003_020d4818(State *);
void func_ov003_020d5298(State *p)
{
    p->counter++;
    if (p->counter > 8) {
        p->flag = 1;
        p->current = p->next;
        if (p->current == 0) func_0203bfb4(p->items[0], 1);
        else func_0203bfb4(p->items[0], 0);
        if (p->current >= p->count - 1) func_0203bfb4(p->items[1], 1);
        else func_0203bfb4(p->items[1], 0);
        int result = 0;
        if (func_0202cb64(func_02010b7c()) == 0) {
            int i = 0;
            do {
                if (p->items[i]->slot54()) result = i + 1;
                i++;
            } while (i < 2);
        }
        unsigned int flags = *(unsigned int *)0x020c9d50u;
        if (flags & 0x100) result = 1;
        if (flags & 0x200) result = 2;
        func_ov003_020d4ce4(p, result);
    }
    func_ov003_020d4818(p);
}
}
