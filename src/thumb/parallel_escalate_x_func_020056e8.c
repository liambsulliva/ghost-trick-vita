struct Channel {
    char pad0[0xc8];
    char list_c8[0x10];
    char list_d8[0x10];
    char pad_e8[9];
    unsigned char playing;
    char pad_f2[2];
    unsigned char fade;
    unsigned char paused;
    unsigned char volume[2];
    unsigned char pan[2];
    unsigned char pitch[2];
    unsigned short time;
    unsigned short length;
    int position;
    int loop_start;
    int loop_end;
    char pad_10c[8];
    int data[2];
    int handle;
    char pad_120[0x284];
};

struct Message {
    int type;
    unsigned char priority;
    unsigned char flag;
    char pad6[2];
    int value;
    char body[0x18];
};

struct State {
    char pad0[0x11e0];
    char buffer_11e0[0x9ec];
    char thread_1bcc[0xd8];
    char threads[4][0xc0];
    char pad_1fa4[0x980];
    char stacks[4][0x400];
    char pad_3924[0x404];
    struct Channel channels[4];
    struct Message messages[4];
    char *buffer;
    int count;
};

struct Root {
    char pad0[0x10];
    struct State *state;
};

extern struct Root data_020b18a0;
extern void func_0200551c(int index);
extern void func_0200584c(void *list, void (*proc)(struct Channel *), struct Channel *channel);
extern void func_02078d80(void *list);
extern void func_02078da0(void *list);
extern void func_02073ed4(void *thread, void (*proc)(struct Channel *), void *arg, void *stack, unsigned int size, unsigned int prio);
extern void func_02072fa8(void *dst, int value, unsigned int size);
extern void func_02074188(void *thread);
extern void func_02074564(void *thread);
extern void func_0200513c(struct Channel *channel);
extern void func_0200514c(struct Channel *channel);
extern void func_02004fcc(struct Channel *channel);

void func_020056e8(void)
{
    unsigned int j;
    int i;
    struct Channel *channel;
    int w;

    i = 0;
    do {
        channel = (struct Channel *)((char *)data_020b18a0.state + 0x3d28 + i * 0x3a4);
        func_0200551c(i);
        channel->paused = 0;
        channel->loop_end = 0;
        channel->time = 0;
        channel->length = 0;
        channel->handle = 0;
        channel->playing = 0;
        channel->position = 0;
        channel->loop_start = 0;
        channel->fade = 0;
        for (j = 0; j < 2; j++) {
            channel->volume[j] = 0xff;
            channel->pan[j] = 0x7f;
            channel->pitch[j] = 0xff;
            channel->data[j] = 0;
        }
        func_0200584c(channel->list_c8, func_0200513c, channel);
        func_0200584c(channel->list_d8, func_0200514c, channel);
        func_02078d80(channel->list_c8);
        func_02078da0(channel->list_d8);

        w = 0xc0;
        w *= i;
        func_02073ed4((char *)data_020b18a0.state->threads + w, func_02004fcc, channel, data_020b18a0.state->stacks[i], 0x400, 0x1f);
        data_020b18a0.state->messages[i].type = 9;
        data_020b18a0.state->messages[i].priority = 3;
        data_020b18a0.state->messages[i].flag = 0;
        data_020b18a0.state->messages[i].value = 0;
        func_02072fa8(data_020b18a0.state->messages[i].body, 0, sizeof(data_020b18a0.state->messages[i].body));
        func_02074188((char *)data_020b18a0.state->threads + w);
        i++;
    } while ((unsigned int)i < 4);
    data_020b18a0.state->buffer = data_020b18a0.state->buffer_11e0;
    func_02074564(data_020b18a0.state->thread_1bcc);
    data_020b18a0.state->count = 0;
}
