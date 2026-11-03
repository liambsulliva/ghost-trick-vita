typedef struct Info02e14 {
    char pad0[4];
    unsigned char group;
    unsigned char sub;
    char pad6[4];
    unsigned char mode;
} Info02e14;

typedef struct Slot02e14 {
    char pad0[0x10];
    unsigned char priority;
    char pad11[0x1f];
    unsigned char channel;
    char pad31[0xf];
    void *owner;
    Info02e14 *info;
    char pad48[4];
} Slot02e14;

typedef struct Bank02e14 {
    char pad[0x15bc];
    Slot02e14 slots[16];
    char pad2[0x104];
    unsigned short count;
} Bank02e14;

typedef struct Game02e14 {
    char pad[0x10];
    Bank02e14 *bank;
} Game02e14;

extern Game02e14 data_020b18a0;

Slot02e14 *func_02002d68(void *owner, unsigned char group, unsigned char sub, unsigned char shared)
{
    Slot02e14 *best;
    int i;
    Slot02e14 *slot;
    unsigned char n;
    unsigned char m;

    best = 0;
    for (i = 0; i < 16; i++) {
        slot = &data_020b18a0.bank->slots[i];
        if (slot->info == (Info02e14 *)-1) {
            continue;
        }
        n = 0;
        m = (owner == slot->owner || owner == 0 || shared == 1) ? 1 : 0;
        n += m;
        m = (group == slot->info->group || group == 0xff) ? 1 : 0;
        n += m;
        m = (sub == slot->info->sub || sub == 0xff) ? 1 : 0;
        n += m;
        if (n == 3) {
            if (best == 0) {
                best = slot;
            } else if (slot->priority <= best->priority) {
                best = slot;
            }
        }
    }
    return best;
}
