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
    char pad0[0xe];
    unsigned char volume;
    char pad0f[2];
    unsigned char group;
    char pad12[6];
    struct Fade fade;
    char pad30[4];
    unsigned short note_on;
    unsigned short note_off;
    int handle;
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
extern unsigned int func_02007a04(void);
extern void func_02007394(unsigned int index, int note, int on);
extern void func_02003a70(unsigned int index);
extern void func_0200799c(unsigned int index);
extern void func_02003b1c(unsigned int index);
extern void func_020079bc(unsigned int index, int volume);
extern int func_02076e08(int handle);
extern void func_02007994(unsigned int index);
extern unsigned int func_0207bd24(unsigned int a, unsigned int b);
extern unsigned int func_0207bab8(unsigned int a);
extern unsigned int func_0207b424(unsigned int a);
extern unsigned int func_0207bb44(unsigned int a, unsigned int b);
extern unsigned int func_0207c1d8(unsigned int a, unsigned int b);
extern int func_0207ba84(unsigned int a);

void func_02003b48(void)
{
    unsigned int mask;
    unsigned int i;
    struct Slot *slot;
    struct Fade *fade;
    struct Track *track;

    mask = func_02007a04();
    for (i = 0; i < 16; i++) {
        slot = &data_020b18a0.state->slots[i];
        if (slot->note_on != 0) {
            func_02007394(i, slot->note_on, 1);
            slot->note_on = 0;
        }
        if (slot->note_off != 0) {
            func_02007394(i, slot->note_off, 0);
            slot->note_off = 0;
        }
        if (slot->flags & 8) {
            fade = &slot->fade;
            track = &data_020b18a0.state->tracks[slot->group];
            if ((slot->flags & 4) == 0) {
                fade->level = func_0207bd24(fade->level, fade->step);
                slot->volume = func_0207bab8(func_0207bd24(0x3f000000, fade->level));
                if (fade->count == 0) {
                    slot->volume = fade->target;
                    if (fade->state == 1) {
                        if (fade->flag == 0) {
                            func_02003a70(i);
                            func_0200799c(i);
                        } else {
                            func_02003b1c(i);
                        }
                    }
                    slot->flags &= ~8;
                    fade->state = 3;
                } else {
                    fade->count--;
                }
            }
            func_020079bc(i, func_0207ba84(func_0207bd24(0x3f000000, func_0207c1d8(func_0207bb44(func_0207b424(slot->volume), func_0207b424(track->volume)), 0x42fe0000))));
        }
        if ((slot->flags & 4) == 0) {
            if ((slot->flags & 3) == 1 && ((mask >> i) & 1) == 0) {
                func_02003a70(i);
                func_0200799c(i);
                slot->flags &= ~8;
                slot->fade.state = 3;
            }
            if (slot->handle != 0 && (slot->flags & 1) == 0 && func_02076e08(slot->handle) != 0) {
                slot->flags |= 1;
                slot->handle = 0;
            }
            if (slot->flags & 2) {
                func_02007994(i);
                slot->flags &= ~2;
            }
        }
    }
}
