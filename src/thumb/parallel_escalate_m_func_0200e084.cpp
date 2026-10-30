class Member0200e084 {
public:
    virtual void slot0();
    virtual void slot1();
    virtual void slot2();
    virtual void slot3();
    virtual int slot4(int value, int flag);
    virtual void slot5();
    virtual void slot6();
    virtual void slot7();
    virtual void slot8();
    virtual void slot9();
    virtual int slot10();
};
extern "C" {
struct Holder0200e084 {
    int word0;
    int state;
    unsigned char pad8[12];
    int id;
    int range;
    int handle;
    int more[2];
    Member0200e084 member;
};
int func_0200e084(struct Holder0200e084 *holder)
{
    if (holder->member.slot4(holder->id, 1) == 0) {
        holder->state = 4;
        return 0;
    }
    holder->range = holder->member.slot10();
    return 1;
}
}
