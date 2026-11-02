struct AnimFlags {
    unsigned char b0 : 1;
    unsigned char b1 : 1;
    unsigned char b2 : 1;
    unsigned char loop : 1;
    unsigned char done : 1;
};

struct Anim {
    int f00;
    unsigned short *frames;
    int index;
    int f0c;
    unsigned short wait;
    struct AnimFlags flags;
    unsigned char count;
    void *target;
};

extern void func_02007cd4(struct Anim *anim, int loop, int start);

void func_02007c14(struct Anim *anim, unsigned short *frames, int loop, int start)
{
    anim->index = 0;
    anim->wait = 0;
    anim->frames = frames;
    anim->flags.loop = 0;
    anim->flags.done = 0;
    if (anim->target != 0) {
        func_02007cd4(anim, loop, start);
    }
}
