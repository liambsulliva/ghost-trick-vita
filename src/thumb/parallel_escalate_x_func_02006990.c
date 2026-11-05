struct Fade {
    int count;
    unsigned int level;
    unsigned int step;
    unsigned char target;
    char pad0d[3];
    int flag;
    int state;
};

struct Slot {
    char pad0[4];
    int kind;
    char pad8[6];
    unsigned char volume;
    char pad0f[2];
    unsigned char group;
    unsigned char owner;
    char pad13[5];
    struct Fade fade;
    char pad30;
    unsigned char base_volume;
    char pad32[0xa];
    unsigned int flags;
    char pad40[0xc];
};

struct Track {
    char pad0[6];
    unsigned char volume;
    char pad7;
};

struct State {
    char pad0[0x15bc];
    struct Slot slots[16];
    struct Track tracks[32];
};

struct Root {
    char pad0[0x10];
    struct State *state;
};

extern struct Root data_020b18a0;
extern unsigned int func_02002ff4(int id);
extern unsigned int func_02003030(int id);
extern unsigned int func_0200306c(int id);
extern void func_02006954(struct Fade *fade, unsigned int from, unsigned int to, int mode, int frames);
extern void func_020079bc(int index, int volume);
extern unsigned int func_0207bd24(unsigned int a, unsigned int b);
extern unsigned int func_0207b3dc(unsigned int a);
extern unsigned int func_0207b424(unsigned int a);
extern unsigned int func_0207bb44(unsigned int a, unsigned int b);
extern unsigned int func_0207c1d8(unsigned int a, unsigned int b);
extern int func_0207ba84(unsigned int a);

int func_02006990(int group, int sound, int bank, unsigned int volume, int frames, int owner)
{
    int i;
    struct Slot *slot;
    int kind;
    int mask;

    mask = 0xffff;
    if (group != -1) {
        mask &= func_02002ff4(group);
    }
    if (sound != -1) {
        mask &= func_02003030(sound);
    }
    if (bank != -1) {
        mask &= func_0200306c(bank);
    }
    switch (volume & 0x80000000) {
    default:
        kind = 1;
        break;
    case 0:
        kind = 0;
        break;
    }
    volume &= 0x7fffffff;
    for (i = 0; i < 16; i++) {
        slot = &data_020b18a0.state->slots[i];
        if ((mask >> i) & 1) {
            if (kind == slot->kind && (slot->owner == owner || owner == 0xff)) {
                struct Fade *fade = &(*(struct State **)((char *)&data_020b18a0 + 0x10))->slots[i].fade;
                struct Track *track = &(*(struct State **)((char *)&data_020b18a0 + 0x10))->tracks[slot->group];
                if (fade != 0) {
                    slot->flags &= ~8;
                    fade->state = 3;
                    slot->base_volume = volume;
                    if (frames != 0) {
                        func_02006954(fade, slot->volume, volume, 2, frames);
                        slot->flags |= 8;
                    } else {
                        slot->volume = volume;
                        func_020079bc(i, func_0207ba84(func_0207bd24(0x3f000000, func_0207c1d8(func_0207bb44(func_0207b3dc(volume), func_0207b424(track->volume)), 0x42fe0000))));
                    }
                }
            }
        }
    }
    return 0;
}
