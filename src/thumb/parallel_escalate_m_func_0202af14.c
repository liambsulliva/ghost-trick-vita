typedef struct {
    unsigned short value;
    unsigned short pad;
    int rest;
} Rec0202af14;

typedef struct {
    unsigned char active : 1;
    unsigned char other : 7;
    unsigned char state;
    unsigned char level;
    unsigned char pad;
    unsigned int time;
    int id;
} Slot0202af14;

typedef struct {
    char pad0[0x34];
    unsigned int time;
    char pad1[0x1e4 - 0x38];
    Slot0202af14 slots[8];
} Obj0202af14;

typedef union {
    unsigned long long key;
    struct {
        int word;
        unsigned char level;
    } s;
} Info0202af14;

extern Rec0202af14 data_02080e84[];
extern Rec0202af14 data_02080e86[];
extern Info0202af14 *func_020073cc(unsigned short a, unsigned short b);
extern void func_0202aba4(Obj0202af14 *obj, int slot, int flag);

int func_0202af14(Obj0202af14 *obj, int id)
{
    Info0202af14 *info;
    unsigned char level;
    unsigned char slot;
    unsigned char i;
    unsigned int best;
    unsigned char min;
    Slot0202af14 *s;

    slot = 0xff;
    level = 0;
    info = func_020073cc(data_02080e86[id].value, data_02080e84[id].value);
    if (info != 0 && info->key != 0) {
        level = info->s.level;
    }
    for (i = 0; i < 8; i++) {
        if (!obj->slots[i].active) {
            slot = i;
            break;
        }
    }
    if (slot == 0xff) {
        min = 0xff;
        best = 0xffffffff;
        for (i = 0; i < 8; i++) {
            if (min > obj->slots[i].level) {
                min = obj->slots[i].level;
            }
        }
        if (level >= min) {
            for (i = 0; i < 8; i++) {
                s = &obj->slots[i];
                if (best > s->time && min == s->level) {
                    best = s->time;
                    slot = i;
                }
            }
        }
    }
    if (slot == 0xff) {
        return slot;
    }
    s = &obj->slots[slot];
    if (obj->slots[slot].active) {
        func_0202aba4(obj, slot, 0);
    }
    s->active = 1;
    s->state = 6;
    s->level = level;
    s->time = obj->time;
    s->id = id;
    return slot;
}
