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
extern Slot02e14 *func_02002d68(void *owner, unsigned char group, unsigned char sub, unsigned char shared);
extern Slot02e14 *func_02002c24(void);
extern void func_0200799c(unsigned int index);
extern void func_02003a70(unsigned int index);
extern void func_02007948(void);

Slot02e14 *func_02002e14(void *owner, Info02e14 *info, unsigned int priority)
{
    int stolen;
    unsigned char group;
    unsigned char shared;
    unsigned char limit;
    unsigned char sub;
    unsigned int count;
    int i;
    unsigned char n;
    unsigned char m;
    Slot02e14 *slot;
    Slot02e14 *result;

    stolen = 0;
    group = info->group;
    count = 0;
    sub = info->sub;
    shared = (info->mode & 0x80) >> 7;
    limit = info->mode & 0x7f;
    for (i = 0; i < 16; i++) {
        slot = &data_020b18a0.bank->slots[i];
        if (slot->info == (Info02e14 *)-1) {
            continue;
        }
        n = 0;
        m = (owner == slot->owner || shared == 1) ? 1 : 0;
        n += m;
        m = (group == slot->info->group || group == 0xff) ? 1 : 0;
        n += m;
        m = (sub == slot->info->sub || sub == 0xff) ? 1 : 0;
        n += m;
        if (n == 3) {
            count++;
        }
    }
    if (limit != 0) {
        for (; count >= limit; count--) {
            slot = func_02002d68(owner, group, sub, shared);
            if (slot == 0) {
                break;
            }
            if (slot->priority <= priority) {
                func_0200799c(slot->channel);
                func_02003a70(slot->channel);
            } else {
                return 0;
            }
        }
    }
    if (data_020b18a0.bank->count < 16) {
        slot = func_02002c24();
        if (slot == 0) {
            result = 0;
            slot = func_02002d68(0, 0xff, 0xff, 1);
            stolen = 1;
            if (slot != 0) {
                if (slot->priority <= priority) {
                    func_02003a70(slot->channel);
                } else {
                    return result;
                }
            } else {
                return result;
            }
        }
    } else {
        result = 0;
        slot = func_02002d68(0, 0xff, 0xff, 1);
        stolen = 1;
        if (slot != 0) {
            if (slot->priority <= priority) {
                func_02003a70(slot->channel);
            } else {
                return result;
            }
        } else {
            return result;
        }
    }
    if (stolen) {
        func_02007948();
    }
    data_020b18a0.bank->count++;
    return slot;
}
