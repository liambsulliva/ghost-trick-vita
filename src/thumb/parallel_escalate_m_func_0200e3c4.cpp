class Member {
public:
    virtual void slot0();
    virtual void slot1();
    virtual void slot2();
    virtual void slot3();
    virtual void slot4();
    virtual void slot5();
};
extern "C" {
struct Manager {
    void (**table)(struct Manager *manager,int handle);
};
struct Holder {
    int handle;
    unsigned char pad4[3];
    unsigned char busy;
    unsigned char active;
    unsigned char pad9[3];
    Member member;
};
extern void *func_02007bd8(void);
extern struct Manager *func_02007b70(void *system);
extern void func_0201ef74(Member *member);
struct Holder *func_0200e3c4(struct Holder *holder)
{
    struct Manager *manager;
    if (holder->handle != 0) {
        if (func_02007b70(func_02007bd8()) != 0) {
            manager = func_02007b70(func_02007bd8());
            manager->table[4](manager,holder->handle);
            holder->handle = 0;
        }
    }
    if (holder->active != 0 && holder->busy == 0) {
        holder->member.slot5();
    }
    func_0201ef74(&holder->member);
    return holder;
}
}
