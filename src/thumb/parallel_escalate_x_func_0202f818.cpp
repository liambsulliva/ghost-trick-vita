class Task {
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
    unsigned char pad4[0x8];
    unsigned char state;
    unsigned char prio;
    unsigned char flags;
    unsigned char padf[0xd];
    int owner;
    Task *parent;
};

class TaskList {
public:
    virtual void slot0();
    virtual void slot1();
    virtual void slot2();
    virtual void slot3();
    virtual void remove(Task *task);
};

struct Manager {
    void *head;
    TaskList list;
    unsigned char pad8[0x11c];
    int count;
};

extern "C" {
extern void *func_02026d10(void);
extern void func_0202fbf8(void *it, int kind);
extern int func_0200eac4(void *it);
extern Task *func_0200eab4(void *it);
extern Task *func_0200eab8(void *it);
extern void func_0200eabc(void *it, int a);
extern unsigned int func_0202f9a8(Task *task);
extern void func_0202fa50(Manager *m, Task *task);
void func_0202f818(Manager *m, int kind, unsigned int prio);
}

void func_0202f818(Manager *m, int kind, unsigned int prio)
{
    void *it;
    int found;
    Task *task;

    it = func_02026d10();
    func_0202fbf8(it, kind);
    found = 0;
    while (func_0200eac4(it) != 0 && func_0202f9a8(func_0200eab4(it)) <= prio) {
        if (func_0200eab4(it)->state != 3) {
            switch (func_0200eab4(it)->state) {
            case 2:
                func_0200eab4(it)->slot5();
                if (func_0200eab4(it)->state == 2) {
                    func_0200eab4(it)->state = 3;
                }
                break;
            case 5:
                func_0200eab4(it)->slot9();
                if (func_0200eab4(it)->state == 5) {
                    func_0200eab4(it)->state = 7;
                }
                break;
            }
        }
        if (func_0200eab4(it)->parent != 0 && func_0200eab4(it)->parent->state == 4) {
            func_0200eab4(it)->state = 4;
        } else if (func_0200eab4(it)->state == 3 && !(func_0200eab4(it)->flags & 4)) {
            func_0200eab4(it)->slot6();
        }
        if (func_0200eab4(it)->state == 4) {
            found = 1;
        }
        if (func_0200eab4(it)->state == 7) {
            func_0200eab4(it)->state = 3;
        }
        func_0200eabc(it, 0);
    }

    if (found) {
        func_0202fbf8(it, kind);
        while (func_0200eac4(it) != 0 && func_0202f9a8(func_0200eab4(it)) <= prio) {
            if (func_0200eab4(it)->state == 4 && func_0200eab4(it)->owner == 0) {
                func_0202fa50(m, func_0200eab8(it));
                task = func_0200eab8(it);
                if (task != 0) {
                    task->slot1();
                }
                m->list.remove(func_0200eab8(it));
                m->count--;
            }
            func_0200eabc(it, 0);
        }
    }
}
