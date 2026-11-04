struct State {
    unsigned int before[4];
    int data;
};

struct Track {
    unsigned char pad0[0x16];
    unsigned short mask;
    unsigned char pad18[0x18];
    unsigned char handle;
};

struct TrackInfo {
    unsigned char pad0[2];
    signed char volume;
    unsigned char pad3[3];
    signed char pan;
    unsigned char pad7[0x15];
};

struct NoteInfo {
    signed char volume;
    signed char pan;
    unsigned char pad2[2];
};

extern struct State data_020b18a0;
extern void func_0207679c(int data);
extern unsigned int func_02076ddc(void);
extern void func_02076b60(int value);
extern void func_02076cfc(unsigned int mask);
extern void func_02077150(int data, int index, int channel, struct TrackInfo *info);
extern void func_020031c4(char *note, char *seq, struct NoteInfo *info);
extern void func_02007a24(unsigned char handle, unsigned short mask, int value);
extern void func_02007a3c(unsigned char handle, unsigned short mask, int value);
extern void func_02007a44(unsigned char handle, unsigned short mask, int value);
extern unsigned int func_0207b3dc(short value);
extern unsigned int func_0207bb44(unsigned int a, unsigned int b);
extern unsigned int func_0207c1d8(unsigned int a, unsigned int b);
extern unsigned int func_0207bd24(unsigned int a, unsigned int b);
extern int func_0207ba84(unsigned int value);

void func_020036e4(int index, unsigned short mask, int flags, short volume, signed char pan, short pitch, char *seq, unsigned short *offsets)
{
    struct Track *track;
    struct TrackInfo info;
    struct NoteInfo note;
    unsigned int saved;
    unsigned int f;
    int i;
    signed char cur_volume;
    int cur_pan;
    int value;

    track = (struct Track *)(data_020b18a0.data + 0x15bc + index * 0x4c);
    if (mask == 0) {
        mask = track->mask;
    }
    if (seq == 0) {
        func_0207679c(data_020b18a0.data);
        saved = func_02076ddc();
        func_02076b60(1);
        func_02076cfc(saved);
    }
    for (i = 0; i < 16; i++) {
        if (((track->mask >> i) & 1) & (mask >> i)) {
            if (seq == 0) {
                func_02077150(data_020b18a0.data, index, i, &info);
                cur_volume = info.volume;
                cur_pan = info.pan;
            } else {
                func_020031c4(seq + offsets[i], seq, &note);
                cur_volume = note.volume;
                cur_pan = note.pan;
            }
            if (flags & 0xe00) {
                if (flags & 0x200) {
                    value = volume;
                } else if (flags & 0x400) {
                    value = cur_volume + volume;
                } else if (flags & 0x800) {
                    value = func_0207b3dc(cur_volume);
                    f = func_0207bb44(value, func_0207b3dc(volume));
                    f = func_0207c1d8(f, 0x42fe0000);
                    value = func_0207ba84(f);
                }
                if (value < 0) {
                    value = 0;
                } else if (value > 0x7f) {
                    value = 0x7f;
                }
                func_02007a24(track->handle, 1 << i, value);
            }
            if (flags & 0x3000) {
                value = ((flags & 0x1000) ? pan - cur_pan : pan) * 2;
                if (value < -0x80) {
                    value = -0x80;
                } else if (value > 0x7f) {
                    value = 0x7f;
                }
                func_02007a3c(track->handle, 1 << i, value);
            }
            if (flags & 0x18000) {
                if (flags & 0x8000) {
                    value = 0;
                } else {
                    f = func_0207b3dc(pitch);
                    f = func_0207bb44(0x42800000, f);
                    f = func_0207c1d8(f, 0x42c80000);
                    f = func_0207bd24(0x3f000000, f);
                    value = func_0207ba84(f);
                }
                if (value < -0x8000) {
                    value = -0x8000;
                } else if (value > 0x7fff) {
                    value = 0x7fff;
                }
                func_02007a44(track->handle, 1 << i, value);
            }
        }
    }
}
