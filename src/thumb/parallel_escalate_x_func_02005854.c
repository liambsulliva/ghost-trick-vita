typedef struct FileId05854 {
    int arch;
    int file;
} FileId05854;

typedef struct Info05854 {
    char pad0[0x24];
    int key;
} Info05854;

typedef struct Archive05854 {
    char pad0[0x7c];
    FileId05854 id;
    char pad84[0x14];
    Info05854 *info;
} Archive05854;

typedef struct Fade05854 {
    int count;
    float cur;
    float step;
    unsigned char target;
    unsigned char mode;
    char padE[2];
    int stop;
    int done;
} Fade05854;

typedef struct Header05854 {
    char pad0[0x18];
    unsigned char format;
    unsigned char loop;
    unsigned char tracks;
    char pad1b;
    unsigned short rate;
    unsigned short timer;
    char pad20[0x20];
} Header05854;

typedef struct Slot05854 {
    char pad0[0x4a];
    short id;
    unsigned int start;
    int skip;
    int mute;
    char pad58[8];
    unsigned char volume;
    char pad61;
    unsigned char thread;
    char pad63;
    unsigned int pos;
    int hist[2];
    Fade05854 fade;
    Header05854 hdr;
    char padc8[0x20];
    unsigned int flags;
    int wait;
    unsigned char state;
    unsigned char cmd;
    char padf2[2];
    unsigned char result;
    unsigned char channels;
    unsigned char chan[2];
    unsigned char vol[2];
    unsigned char pan[2];
    unsigned short period;
    unsigned short step;
    char pad100[8];
    int timer;
    int alarm;
    int frames;
    int buf[2];
    int len;
    char pad120[0x280];
    unsigned char head;
    unsigned char tail;
    char pad3a2[2];
} Slot05854;

typedef struct Thread05854 {
    char data[0xc0];
} Thread05854;

typedef struct Event05854 {
    char pad0;
    unsigned char volume;
    char pad2[2];
    unsigned int flags;
    Fade05854 fade;
    int pad20;
} Event05854;

typedef struct Bank05854 {
    char pad0[0x1580];
    Archive05854 *archive;
    char pad1584[0x720];
    Thread05854 threads[4];
    char pad1fa4[0x1d84];
    Slot05854 slots[4];
    char pad4bb8[4];
    Event05854 events[4];
} Bank05854;

typedef struct Game05854 {
    char pad0[0x10];
    Bank05854 *bank;
} Game05854;

extern Game05854 data_020b18a0;
extern unsigned char data_020b18a4[];
extern int *func_02000d9c(Archive05854 *arc, int name, int key);
extern int func_02000d64(Archive05854 *arc, int file, void *dst, unsigned int size, int zero);
extern int func_0206c538(Slot05854 *file, FileId05854 id);
extern void func_020766c8(int chan, int format, int data, int loop, int start, unsigned int len, int volume, int shift, int timer, int pan);
extern void func_020766a8(unsigned int mask, int pan);
extern void func_020055fc(Slot05854 *slot, int volume);
extern void func_020765d0(int alarm, unsigned int tick, unsigned int period, void (*callback)(void *arg), void *arg);
extern void func_020050d0(void *arg);
extern void func_02074188(Thread05854 *thread);
extern void func_0207303c(const void *src, void *dst, unsigned int size);
extern void func_02007a84(unsigned int a, unsigned int b, unsigned int c, unsigned int d);
extern void func_02007a70(unsigned int mask, unsigned int pan);
extern float func_0207bd24(float a, float b);
extern unsigned int func_0207bab8(float value);
extern void func_02005654(short id);
extern void func_020062ec(Slot05854 *slot, short id, unsigned char ch);

void func_02005854(void)
{
    unsigned int i;
    Slot05854 *slot;
    int *entry;
    unsigned char fmt;
    unsigned char pan;
    unsigned char n;
    int len;
    unsigned int tick;
    unsigned int j;
    Fade05854 *fade;

    for (i = 0; i < 4; i++) {
        slot = &data_020b18a0.bank->slots[i];
        if (!(slot->flags & 1)) {
            continue;
        }
        switch (slot->state) {
        case 0:
            entry = func_02000d9c(data_020b18a0.bank->archive, slot->id, data_020b18a0.bank->archive->info->key);
            if (entry == 0) {
                continue;
            }
            if ((unsigned int)(func_02000d64(data_020b18a0.bank->archive, *entry, &slot->hdr, 0x40, 0) + 3) <= 1) {
                return;
            }
            if (func_0206c538(slot, data_020b18a0.bank->archive->id) == 0) {
                return;
            }
            slot->pos = slot->hdr.rate * slot->start / 1000;
            if (slot->start != 0 && slot->hdr.format == 2) {
                slot->skip = 1;
            } else {
                slot->skip = 0;
            }
            for (i = 0; i < slot->channels; i++) {
                fmt = slot->hdr.format;
                if (fmt == 2) {
                    fmt = 1;
                }
                pan = slot->pan[i];
                if (pan == -1) {
                    pan = 0x40;
                }
                func_020766c8(slot->chan[i], fmt, slot->buf[i], 1, 0, (unsigned int)slot->len >> 2, slot->vol[i], 0, slot->hdr.timer << 5, pan);
            }
            n = slot->hdr.tracks;
            if (((unsigned char *)entry)[7] != 0) {
                n = 2;
            }
            if (n > slot->channels) {
                n = slot->channels;
            }
            slot->mute = n == 1;
            if (n == 2) {
                func_020766a8(1 << slot->chan[0], 0);
                func_020766a8(1 << slot->chan[1], 0x7f);
                if (slot->pan[0] == -1) {
                    slot->pan[0] = 0;
                }
                if (slot->pan[1] == -1) {
                    slot->pan[1] = 0x7f;
                }
            } else {
                if (slot->pan[0] == -1) {
                    slot->pan[0] = 0x40;
                }
                if (slot->pan[1] == -1) {
                    slot->pan[1] = 0x40;
                }
            }
            func_020055fc(slot, slot->volume);
            len = slot->len;
            if (slot->hdr.format == 1 || slot->hdr.format == 2) {
                len >>= 1;
            }
            tick = (unsigned int)(slot->hdr.timer * len) / slot->period * slot->step;
            func_020765d0(slot->alarm, tick, tick, func_020050d0, slot);
            slot->state = 1;
            break;
        case 1:
            func_02074188(&data_020b18a0.bank->threads[slot->thread]);
            data_020b18a4[i] += (unsigned char)slot->frames;
            if (data_020b18a4[i] >= slot->period) {
                data_020b18a4[i] = 0;
                if (slot->flags & 8) {
                    slot->wait = 10;
                    slot->state = 3;
                } else {
                    slot->state = 2;
                }
            }
            break;
        case 3:
            if (slot->flags & 8) {
                slot->result = 0;
                break;
            }
        case 2:
            if (data_020b18a0.bank->events[i].flags & 2) {
                data_020b18a0.bank->events[i].flags &= ~2;
                slot->volume = data_020b18a0.bank->events[i].volume;
                slot->flags |= data_020b18a0.bank->events[i].flags;
                slot->fade.mode = data_020b18a0.bank->events[i].fade.mode;
                slot->fade.done = data_020b18a0.bank->events[i].fade.done;
                func_0207303c(&data_020b18a0.bank->events[i].fade, &slot->fade, 0x18);
                func_020055fc(slot, slot->volume);
            } else if (slot->flags & 4) {
                slot->fade.mode = 7;
                slot->fade.done = 0;
            }
            slot->state = 4;
            slot->wait = 6;
            func_02007a84(slot->timer, 0, 1 << slot->alarm, 0);
            func_02074188(&data_020b18a0.bank->threads[slot->thread]);
            break;
        case 4:
            func_02074188(&data_020b18a0.bank->threads[slot->thread]);
            break;
        case 5:
            break;
        }
        if (slot->cmd != 0) {
            if (slot->cmd & 1) {
                func_020055fc(slot, slot->volume);
            }
            if (slot->cmd & 2) {
                for (j = 0; j < 2; j++) {
                    func_02007a70(1 << slot->chan[j], slot->pan[j]);
                }
            }
            slot->cmd = 0;
        }
        switch (slot->fade.mode) {
        case 6:
            slot->fade.done = 1;
            slot->fade.mode = 7;
        case 7:
            fade = &slot->fade;
            fade->cur = func_0207bd24(fade->cur, fade->step);
            slot->volume = func_0207bab8(func_0207bd24(0.5f, fade->cur));
            if (slot->volume > 0x7f) {
                slot->volume = 0x7f;
            }
            if (fade->count == 0) {
                slot->volume = fade->target;
                if (fade->done == 1) {
                    if (fade->stop == 0) {
                        func_02005654(slot->id);
                        slot->state = 5;
                        slot->wait = 9;
                    } else {
                        func_020062ec(slot, slot->id, i);
                    }
                }
                slot->flags &= ~4;
                fade->done = 3;
                fade->mode = 8;
            } else {
                fade->count--;
            }
            func_020055fc(slot, slot->volume);
            break;
        case 8:
            break;
        }
    }
}
