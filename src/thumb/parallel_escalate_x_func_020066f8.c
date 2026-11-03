struct Stream {
    int pad0;
    int capture;
    unsigned int rate;
    int format;
    int mono;
    int pad14;
    void *left;
    void *right;
    unsigned int size;
    int pad24;
    int channels;
    int pad2c;
    int captures;
    int alarm;
    int blocks;
    int looped;
    char pad40[0x24];
    int volume;
};

struct State {
    char pad0[0x4c50];
    struct Stream stream;
};

struct Root {
    char pad0[0x10];
    struct State *state;
};

extern struct Root data_020b18a0;
extern void func_0207680c(unsigned int mask);
extern void func_020766c8(int channel, int format, void *data, int loop, int start, unsigned int length, int volume, int shift, int timer, int pan);
extern void func_02076590(int capture, int format, void *data, unsigned int length, int loop, int add, int source);
extern void func_020765d0(int alarm, int tick, int period, void (*callback)(struct Stream *), struct Stream *arg);
extern void func_02076734(int a, int b, int c, int d);
extern void func_02007a84(int channels, int captures, int alarms, int flag);
extern void func_020064f4(struct Stream *stream);

int func_020066f8(void)
{
    struct Stream *stream;
    int timer;
    int period;
    int tick;
    int step;
    int length;
    int format;
    int pcm;
    int capture;
    int pan_left;
    int pan_right;
    int add;

    stream = &data_020b18a0.state->stream;
    func_0207680c(0x3000);
    timer = 0xffb0ff / stream->rate;
    if (stream->looped != 0) {
        length = stream->size;
        format = stream->format;
        if (format == 2) {
            length >>= 1;
        }
        timer = (timer + 0x10) & ~0x1f;
        period = timer >> 5;
        tick = length / stream->blocks * period;
        step = 0x20;
        if (format == 2) {
            step >>= 1;
        }
        step = step * period;
    }
    switch (stream->format) {
    case 1:
        pcm = 0;
        break;
    default:
        pcm = 1;
        break;
    }
    capture = stream->format == 1 ? 1 : 0;
    pan_left = 0;
    if (stream->mono != 0) {
        pan_left = 0x40;
    }
    pan_right = stream->mono == 0 ? 0x7f : 0x40;
    if (stream->mono == 0) {
        add = 0;
        func_020766c8(1, pcm, stream->left, 1, 0, stream->size >> 2, stream->volume, 0, timer, pan_left);
        if (stream->mono != 0) {
            add = 1;
        }
        func_02076590(0, capture, stream->left, stream->size >> 2, 1, add, 0);
    }
    if (stream->mono == 0) {
        add = 0;
        func_020766c8(3, pcm, stream->right, 1, 0, stream->size >> 2, stream->volume, 0, timer, pan_right);
        if (stream->mono != 0) {
            add = 1;
        }
        func_02076590(1, capture, stream->right, stream->size >> 2, 1, add, 0);
    }
    if (stream->alarm >= 0) {
        func_020765d0(stream->alarm, tick + step, tick, func_020064f4, stream);
    }
    if (stream->capture == 1) {
        func_02076734(3, 3, 1, 1);
    }
    func_02007a84(stream->channels, stream->captures, stream->alarm >= 0 ? 1 << stream->alarm : 0, 0);
    return 0;
}
