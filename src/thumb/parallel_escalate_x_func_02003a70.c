typedef struct Slot03a70 {
    short first;
    short last;
    int mode;
    int pad8;
    unsigned short frame;
    unsigned char wait;
    unsigned char loop;
    unsigned char priority;
    unsigned char group;
    char pad12[0x20];
    unsigned short flags;
    char pad34[4];
    int time;
    int step;
    int next;
    int prev;
    int state;
} Slot03a70;

typedef struct Group03a70 {
    unsigned short mask;
    unsigned char pad2;
    unsigned char count;
    char pad4[4];
} Group03a70;

typedef struct Bank03a70 {
    char pad[0x15bc];
    Slot03a70 slots[16];
    Group03a70 groups[32];
    unsigned char active;
    char pad1b7d;
    short used;
    unsigned short pending;
} Bank03a70;

typedef struct Game03a70 {
    char pad[0x10];
    Bank03a70 *bank;
} Game03a70;

extern Game03a70 data_020b18a0;

void func_02003a70(int index)
{
    Slot03a70 *slot;

    slot = &data_020b18a0.bank->slots[index];
    data_020b18a0.bank->groups[slot->group].mask &= ~(1 << index);
    if (data_020b18a0.bank->groups[slot->group].count != 0) {
        data_020b18a0.bank->groups[slot->group].count--;
    }
    slot->flags = 0;
    slot->time = 0;
    slot->step = 0;
    slot->state = 2;
    slot->first = -1;
    slot->mode = 3;
    slot->last = -1;
    slot->frame = 0;
    slot->loop = 0;
    slot->priority = 0;
    slot->group = 0xff;
    slot->wait = 0;
    slot->next = -1;
    slot->prev = -1;
    data_020b18a0.bank->used &= ~(1 << index);
    if (data_020b18a0.bank->active != 0) {
        data_020b18a0.bank->active--;
    }
    if (data_020b18a0.bank->pending != 0) {
        data_020b18a0.bank->pending--;
    }
}
