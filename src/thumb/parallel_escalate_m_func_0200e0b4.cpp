class Member {
public:
    virtual void slot0();
    virtual void slot1();
    virtual void slot2();
    virtual void slot3();
    virtual void slot4();
    virtual void slot5();
    virtual void slot6();
    virtual void slot7(int handle,int range);
};
extern "C" {
struct Manager {
    int (**table)(struct Manager *manager,int value,void *name);
};
struct Holder {
    int words[2];
    unsigned char pad8;
    unsigned char enabled;
    unsigned char pad10[14];
    int range;
    int handle;
    int more[2];
    Member member;
};
extern char data_02083cc8;
extern int func_0200e084(struct Holder *holder,unsigned char enabled);
extern void *func_02007bd8(void);
extern struct Manager *func_02007b70(void *system);
int func_0200e0b4(struct Holder *holder,unsigned char enabled)
{
    struct Manager *manager;
    int handle;
    holder->enabled = enabled;
    if (func_0200e084(holder,enabled) != 0) {
        if (holder->enabled != 0) {
            manager = func_02007b70(func_02007bd8());
            handle = manager->table[2](manager,-holder->range,&data_02083cc8);
            holder->handle = handle;
            holder->member.slot7(handle,holder->range);
            holder->member.slot5();
        }
        return 1;
    }
    return 0;
}
}
