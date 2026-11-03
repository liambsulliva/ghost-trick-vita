struct State {
    unsigned int before[4];
    int data;
};

struct Entry {
    int index;
    unsigned short sub;
    unsigned char volume;
    unsigned char pan;
    unsigned char prio;
    unsigned char player;
};

struct Group {
    unsigned char pad0[4];
    unsigned short limit;
    unsigned char pad6[2];
};

struct Track {
    unsigned short id;
    unsigned char pad2[2];
    int w4;
    int w8;
    unsigned short sub;
    unsigned char volume;
    unsigned char pan;
    unsigned char prio;
    unsigned char player;
    unsigned char rank;
    unsigned char pad13[3];
    unsigned short mask;
    int fade_time;
    unsigned int fade_from;
    unsigned int fade_step;
    unsigned char fade_target;
    unsigned char pad25[3];
    int w28;
    int w2c;
    unsigned char handle;
    unsigned char level;
    unsigned char pad32[6];
    unsigned int stamp;
    int state;
    unsigned char pad40[8];
    int w48;
};

struct SeqInfo {
    unsigned short mask;
    unsigned short w2;
    unsigned short offsets[16];
};

extern struct State data_020b18a0;
extern int func_02000d9c();
extern void func_0207c594(void *object, int size);
extern struct Track *func_02002c74(unsigned char player, int prio);
extern void func_02003138(int seq, int index, struct SeqInfo *info);
extern unsigned int func_02076ddc(void);
extern unsigned int func_0207b424(unsigned int value);
extern unsigned int func_0207b3dc(int value);
extern unsigned int func_0207bb44(unsigned int a, unsigned int b);
extern unsigned int func_0207c1d8(unsigned int a, unsigned int b);
extern int func_0207ba84(unsigned int value);
extern void func_02007984(int value);
extern void func_0200798c(unsigned char handle, int seq, int a, int bank);
extern void func_02007a14(unsigned char handle, unsigned short mask, unsigned short limit);
extern void func_02003624(unsigned char handle, int flags, int prio, unsigned char pan, unsigned char level);
extern void func_020036e4(int index, int mask, int flags, int volume, signed char pan, int pitch, char *seq, unsigned short *offsets);

int func_02004814(int id, unsigned short mask, int flags, unsigned char prio, unsigned char pan, unsigned char volume, short vol2, signed char pan2, short pitch2, int fade, unsigned char rank)
{
    struct SeqInfo info;
    int wave;
    struct Group *group;
    int seq;
    struct Entry *entry;
    struct Entry *sub;
    struct Track *track;
    unsigned int f;
    int level;

    func_0207c594(&info, 0x24);
    entry = (struct Entry *)func_02000d9c(*(int *)(data_020b18a0.data + 0x1580), id, *(int *)(*(int *)(*(int *)(data_020b18a0.data + 0x1580) + 0x98) + 8));
    if (entry == 0) {
        return -5;
    }
    seq = *(int *)(*(int *)(*(int *)(data_020b18a0.data + 0x1580) + 0x90) + entry->index * 16 + 0x14);
    if (seq == 0) {
        return -5;
    }
    sub = (struct Entry *)func_02000d9c(*(int *)(data_020b18a0.data + 0x1580), entry->sub, *(int *)(*(int *)(*(int *)(data_020b18a0.data + 0x1580) + 0x98) + 0x10));
    if (sub == 0) {
        return -5;
    }
    wave = *(int *)(*(int *)(*(int *)(data_020b18a0.data + 0x1580) + 0x90) + sub->index * 16 + 0x14);
    if (wave == 0) {
        return -5;
    }
    track = func_02002c74(entry->player, (flags & 8) ? prio : entry->prio + prio);
    if (track == 0) {
        return -5;
    }
    group = (struct Group *)(data_020b18a0.data + 0x1a7c) + entry->player;
    track->w48 = 0;
    track->id = id;
    track->rank = rank;
    track->w4 = 0;
    track->sub = entry->sub;
    track->player = entry->player;
    track->volume = entry->volume;
    track->prio = entry->prio;
    track->pan = entry->pan;
    func_02003138(seq + *(int *)(seq + 0x18), 0, &info);
    track->mask = info.mask;
    track->w8 = info.w2;
    track->stamp = func_02076ddc();
    track->state = 3;
    if ((flags & 7) == 0) {
        flags |= 1;
        volume = track->volume;
    }
    if ((flags & 0x18) == 0) {
        flags |= 8;
        prio = track->prio;
    }
    if ((flags & 0xc0) == 0) {
        flags |= 0x40;
        pan = track->pan;
    }
    if (flags & 1) {
        level = volume;
    } else if (flags & 2) {
        level = track->volume + volume;
    } else if (flags & 4) {
        f = func_0207b424(track->volume);
        f = func_0207bb44(f, func_0207b3dc(volume));
        f = func_0207c1d8(f, 0x42fe0000);
        level = func_0207ba84(f);
    }
    if (level < 0) {
        level = 0;
    } else if (level > 0x7f) {
        level = 0x7f;
    }
    track->level = level;
    track->state &= ~8;
    track->w2c = 3;
    if (fade != 0) {
        track->state |= 8;
        track->fade_time = fade;
        track->fade_target = track->level;
        track->volume = 0;
        track->fade_from = func_0207b424(track->volume);
        track->fade_step = func_0207c1d8(func_0207b3dc(track->fade_target - track->volume), func_0207b424(track->fade_time));
        track->w28 = 0;
        track->w2c = 0;
    }
    if (group->limit != 0) {
        func_02007984(2);
    }
    func_0200798c(track->handle, seq + *(int *)(seq + 0x18), 0, wave);
    if (group->limit != 0) {
        func_02007a14(track->handle, 0xffff, group->limit);
    }
    func_02003624(track->handle, flags, prio, pan, track->level);
    func_020036e4(track->handle, mask, flags, vol2, pan2, pitch2, (char *)(seq + *(int *)(seq + 0x18)), info.offsets);
    return 0;
}
