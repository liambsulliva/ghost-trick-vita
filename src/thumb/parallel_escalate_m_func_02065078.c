struct Mover02065078 {
    unsigned char pad0[0x10];
    unsigned char state;
    unsigned char pad11[0x1b];
    int wait;
    int step;
    int reset;
    unsigned char pad38[0x48];
    int value;
    unsigned int limit;
};

extern void func_02065400(struct Mover02065078 *mover);
extern unsigned int func_02064854(int value);

void func_02065078(struct Mover02065078 *mover)
{
    if (mover->wait != 0) {
        mover->wait = mover->wait - 1;
        return;
    }
    switch (mover->state) {
    case 0:
        break;
    case 2:
        mover->value = mover->value + mover->step;
        func_02065400(mover);
        if (func_02064854(mover->value) > mover->limit) {
            mover->state = 3;
            return;
        }
        break;
    case 3:
        break;
    }
    mover->step = mover->reset;
}
