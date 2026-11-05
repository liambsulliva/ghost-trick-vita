struct Fade {
    int value;
    unsigned int start;
    unsigned int rate;
    unsigned char index;
    unsigned char mode;
    char pad0e[2];
    int step80;
    int step84;
};

struct Channel {
    char pad00[0x60];
    unsigned char steps;
    char pad61[0xf];
    struct Fade fade;
    char pad88[0x60];
    unsigned int flags;
    int state;
    unsigned char phase;
    unsigned char padf1;
    unsigned char volume;
    char padf3[0xd];
    int pos100;
    int pos104;
    char pad108[0x3a4 - 0x108];
};

struct Save {
    int state;
    unsigned char phase;
    unsigned char steps;
    char pad6[2];
    unsigned int flags;
    struct Fade fade;
};

struct Game {
    char pad0[0x1580];
    int obj;
    char pad1584[0x3d28 - 0x1584];
    struct Channel channels[4];
    struct Save saves[4];
};

struct Entry {
    char pad0[6];
    unsigned char slot;
};

struct State {
    unsigned int before[4];
    struct Game *game;
};
extern struct State data_020b18a0;
extern struct Entry *func_02000d9c(int obj, int key, int c);
extern void func_02005cec(int key, int a, int b, int c, int d, unsigned char volume);
extern void func_0207303c(const void *src, void *dst, unsigned int size);
extern unsigned int func_0207b424(unsigned int value);
extern unsigned int func_0207b3dc(unsigned int value);
extern unsigned int func_0207c1d8(unsigned int first, unsigned int second);
extern void func_020062ec(struct Channel *channel, int key, int slot);

int func_0200611c(int key, int frames)
{
    struct Entry *src;
    struct Channel *channel;
    unsigned int flags;
    int obj;

    obj = data_020b18a0.game->obj;
    src = func_02000d9c(obj, key, *(int *)(*(int *)(obj + 0x98) + 0x24));
    if (src == 0) {
        return -5;
    }
    channel = &data_020b18a0.game->channels[src->slot];
    flags = channel->flags;
    if ((flags & 1) == 0) {
        return -6;
    }
    if (channel->state == 9) {
        return -8;
    }
    if (flags & 8) {
        channel->pos100 = 0;
        channel->pos104 = 0;
        func_02005cec(key, 0, frames, -1, 2, channel->volume);
    } else {
        data_020b18a0.game->saves[src->slot].state = *(int *)((int)channel + 0xec);
        data_020b18a0.game->saves[src->slot].phase = channel->phase;
        data_020b18a0.game->saves[src->slot].flags = channel->flags;
        data_020b18a0.game->saves[src->slot].flags |= (channel->flags & 4) ? 2 : 0;
        data_020b18a0.game->saves[src->slot].steps = channel->steps;
        data_020b18a0.game->saves[src->slot].fade.mode = channel->fade.mode;
        data_020b18a0.game->saves[src->slot].fade.step84 = channel->fade.step84;
        func_0207303c(&channel->fade, &data_020b18a0.game->saves[src->slot].fade, 0x18);
        if (frames != 0) {
            channel->fade.value = frames;
            channel->fade.start = func_0207b424(channel->steps);
            channel->fade.index = 0;
            channel->fade.rate = func_0207c1d8(func_0207b3dc(channel->fade.index - channel->steps), func_0207b424(channel->fade.value));
            channel->fade.step80 = 1;
            channel->fade.step84 = 1;
            channel->fade.mode = 6;
            channel->flags |= 4;
            channel->phase = 5;
        } else {
            func_020062ec(channel, key, src->slot);
        }
    }
    return 0;
}
