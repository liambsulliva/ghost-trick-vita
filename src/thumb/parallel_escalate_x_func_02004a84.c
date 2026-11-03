struct Entry {
    int offset;
    unsigned short wave;
    unsigned char volume;
    unsigned char pan;
    unsigned char pitch;
    unsigned char channel;
    unsigned char pad[2];
};

struct Seq {
    char pad0[0x18];
    int data;
    char pad1c[4];
    struct Entry entries[1];
};

struct Chan {
    int w0;
    unsigned short active;
    unsigned short pad6;
};

struct Track {
    unsigned short id;
    unsigned short sub;
    int mode;
    int length;
    unsigned short wave;
    unsigned char volume;
    unsigned char pan;
    unsigned char pitch;
    unsigned char channel;
    unsigned char group;
    char pad13[3];
    unsigned short info;
    int fade_frames;
    unsigned int fade_cur;
    unsigned int fade_step;
    unsigned char fade_target;
    char pad25[3];
    int fade_time;
    int state;
    unsigned char slot;
    unsigned char level;
    char pad32[6];
    int stamp;
    int flags;
    char pad40[8];
    int w48;
};

extern char data_020b18a0;
extern void func_0207c594(void *object, int size);
extern int func_02000d9c(int object, int index, int offset);
extern struct Track *func_02002c74(int channel, int pitch);
extern void func_02003138(void *data, int offset, void *buf);
extern int func_02076ddc(void);
extern unsigned int func_0207b424(unsigned int a);
extern unsigned int func_0207b3dc(int a);
extern unsigned int func_0207bb44(unsigned int a, unsigned int b);
extern unsigned int func_0207c1d8(unsigned int a, unsigned int b);
extern int func_0207ba84(unsigned int a);
extern void func_02007984(int value);
extern void func_0200798c(int slot, void *data, int offset, void *wave);
extern void func_02007a14(int slot, int tracks, int value);
extern void func_02003624(int slot, int flags, int a, int b, int level);
extern void func_020036e4(int slot, int a, int flags, int b, int c, int d, void *e, void *f);

int func_02004a84(int id, int sub, unsigned char p2, int flags, unsigned char pitch, unsigned char pan, unsigned char volume, short p7, signed char p8, short p9, int fade, unsigned char group)
{
    unsigned short buf[18];
    void *wave;
    struct Chan *chan;
    struct Seq *seq;
    int *res;
    struct Entry *entry;
    struct Track *track;
    int v;

    func_0207c594(buf, 0x24);
    res = (int *)func_02000d9c(*(int *)(*(int *)((char *)&data_020b18a0 + 0x10) + 0x1580), id, *(int *)(*(int *)(*(int *)(*(int *)((char *)&data_020b18a0 + 0x10) + 0x1580) + 0x98) + 0xc));
    if (res == 0) {
        return -5;
    }
    seq = *(struct Seq **)(*(int *)(*(int *)(*(int *)((char *)&data_020b18a0 + 0x10) + 0x1580) + 0x90) + *res * 0x10 + 0x14);
    if (seq == 0) {
        return -5;
    }
    entry = &seq->entries[sub];
    if (entry == 0) {
        return -5;
    }
    if (entry->offset == -1) {
        return -5;
    }
    res = (int *)func_02000d9c(*(int *)(*(int *)((char *)&data_020b18a0 + 0x10) + 0x1580), entry->wave, *(int *)(*(int *)(*(int *)(*(int *)((char *)&data_020b18a0 + 0x10) + 0x1580) + 0x98) + 0x10));
    if (res == 0) {
        return -5;
    }
    wave = *(void **)(*(int *)(*(int *)(*(int *)((char *)&data_020b18a0 + 0x10) + 0x1580) + 0x90) + *res * 0x10 + 0x14);
    if (wave == 0) {
        return -5;
    }
    track = func_02002c74(entry->channel, (flags & 8) ? pitch : entry->pitch + pitch);
    if (track == 0) {
        return -5;
    }
    chan = (struct Chan *)(*(int *)((char *)&data_020b18a0 + 0x10) + 0x1a7c) + entry->channel;
    track->w48 = 0;
    track->id = id;
    track->mode = 1;
    track->sub = sub;
    track->group = group;
    track->wave = entry->wave;
    track->channel = entry->channel;
    track->volume = entry->volume;
    track->pitch = entry->pitch;
    track->pan = entry->pan;
    func_02003138((char *)seq + seq->data, entry->offset, buf);
    track->info = buf[0];
    track->length = buf[1];
    track->stamp = func_02076ddc();
    track->flags = 3;
    if ((flags & 7) == 0) {
        flags |= 1;
        volume = track->volume;
    }
    if ((flags & 0x18) == 0) {
        flags |= 8;
        pitch = track->pitch;
    }
    if ((flags & 0xc0) == 0) {
        flags |= 0x40;
        pan = track->pan;
    }
    if (flags & 1) {
        v = volume;
    } else if (flags & 2) {
        v = track->volume + volume;
    } else if (flags & 4) {
        v = func_0207ba84(func_0207c1d8(func_0207bb44(func_0207b424(track->volume), func_0207b3dc(volume)), 0x42fe0000));
    }
    if (v < 0) {
        v = 0;
    } else if (v > 0x7f) {
        v = 0x7f;
    }
    track->level = v;
    track->flags &= ~8;
    track->state = 3;
    if (fade != 0) {
        track->flags |= 8;
        track->fade_frames = fade;
        track->fade_target = track->level;
        track->volume = 0;
        track->fade_cur = func_0207b424(track->volume);
        track->fade_step = func_0207c1d8(func_0207b3dc(track->fade_target - track->volume), func_0207b424(track->fade_frames));
        track->fade_time = 0;
        track->state = 0;
    }
    if (chan->active != 0) {
        func_02007984(2);
    }
    func_0200798c(track->slot, (char *)seq + seq->data, entry->offset, wave);
    if (chan->active != 0) {
        func_02007a14(track->slot, 0xffff, chan->active);
    }
    func_02003624(track->slot, flags, pitch, pan, track->level);
    func_020036e4(track->slot, p2, flags, p7, p8, p9, (void *)(entry->offset + ((int)seq + seq->data)), &buf[2]);
    return 0;
}
