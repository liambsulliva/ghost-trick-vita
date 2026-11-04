struct Slot {
    char pad0[4];
    int kind;
    char pad8[6];
    unsigned char volume;
    char pad0f[3];
    unsigned char owner;
    char pad13[0x1e];
    unsigned char base_volume;
    char pad32[0x1a];
};

struct State {
    char pad0[0x15bc];
    struct Slot slots[16];
};

struct Root {
    char pad0[0x10];
    struct State *state;
};

extern struct Root data_020b18a0;
extern unsigned short func_020030a8(void *list);
extern unsigned short func_02003030(int id);
extern unsigned short func_0200306c(int id);
extern unsigned short func_020030e4(int kind, int group);
extern void func_02003624(int index, int flags, unsigned char a, unsigned char b, unsigned char volume);
extern void func_020036e4(int index, unsigned char sound, int flags, short a, signed char b, short c, int d, int e);
extern unsigned int func_0207b3dc(int a);
extern unsigned int func_0207b424(unsigned int a);
extern unsigned int func_0207bb44(unsigned int a, unsigned int b);
extern unsigned int func_0207c1d8(unsigned int a, unsigned int b);
extern int func_0207ba84(unsigned int a);


void func_020043ac(void *list, int sound, int bank, unsigned char kind, unsigned char group, unsigned char track, int flags, unsigned char pitch, unsigned char pan, int volume, short a, signed char b, short c, unsigned char owner)
{
    unsigned short mask;
    short i;
    struct Slot *slot;
    int level;

    mask = 0xffff;
    if (list != 0) {
        mask &= func_020030a8(list);
    }
    if (sound != -1) {
        mask &= func_02003030(sound);
    }
    if (bank != -1) {
        mask &= func_0200306c(bank);
    }
    if (kind != 0xff || group != 0xff) {
        mask &= func_020030e4(kind, group);
    }
    for (i = 0; i < 16; i++) {
        slot = &data_020b18a0.state->slots[i];
        if ((mask >> i) & 1) {
            if (owner == slot->owner || owner == 0xff) {
                if (slot->kind == 1) {
                    if (flags & 2) {
                        level = slot->volume + volume;
                    } else if (flags & 4) {
                        level = func_0207ba84(func_0207c1d8(func_0207bb44(func_0207b424(slot->volume), func_0207b3dc(volume)), 0x42fe0000));
                    } else {
                        level = volume;
                    }
                    if (level < 0) {
                        level = 0;
                    } else if (level > 0x7f) {
                        level = 0x7f;
                    }
                    slot->base_volume = level;
                    func_02003624(i, flags, pitch, pan, slot->base_volume);
                    func_020036e4(i, track, flags, a, b, c, 0, 0);
                }
            }
        }
    }
}

