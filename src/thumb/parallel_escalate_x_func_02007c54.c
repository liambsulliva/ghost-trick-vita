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

int func_02007c54(struct Anim *anim)
{
    if (anim->flags.done) {
        return 0;
    }
    if (anim->target != 0 && anim->frames != 0 && anim->frames[anim->index] != 0xfffe) {
        if (anim->wait != 0) {
            anim->wait--;
            return 0;
        }
        if (anim->wait == 0) {
            if (anim->flags.loop && *(anim->frames + anim->index + 1) == 0xffff) {
                if (anim->count != 0) {
                    anim->count--;
                }
                if (anim->count == 0) {
                    anim->flags.done = 1;
                    anim->flags.loop = 0;
                    return 0;
                }
            }
            anim->index++;
            func_02007cd4(anim, 0, -1);
        }
    }
    return 0;
}
