struct Stream {
    unsigned int flags;
    unsigned int word4;
    unsigned int rate;
    unsigned int channels;
    unsigned int word10;
    unsigned char kind;
    unsigned char pad15[3];
    unsigned char *buffer;
    unsigned char *second;
    unsigned int size;
    unsigned int words24[15];
    unsigned int state;
    unsigned int owner;
};
extern char data_020b18a0;
extern void func_02001fa0(unsigned char *buffer,unsigned int size);
extern void func_020066f8(void);
int func_020068c0(unsigned int owner,unsigned int time)
{
    struct Stream *stream;
    unsigned int size;
    unsigned char *second;
    stream = (struct Stream *)(*(int *)(((int)&data_020b18a0) + 0x10) + 0x4c50);
    if (stream->state != 3) {
        return -8;
    }
    size = stream->rate * stream->channels * time / 1000;
    if (size > stream->size) {
        return -10;
    }
    stream->size = size;
    if (stream->kind == 2) {
        second = stream->buffer + stream->size;
    }
    else {
        second = 0;
    }
    stream->second = second;
    stream->owner = owner;
    stream->flags = stream->flags | 1;
    func_02001fa0(stream->buffer,stream->size);
    if (stream->second != 0) {
        func_02001fa0(stream->second,stream->size);
    }
    func_020066f8();
    return 0;
}
