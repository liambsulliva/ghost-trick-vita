struct Track {
    char pad[0x18];
    unsigned int last;
    int current;
    unsigned int limit;
    int pad24;
    int moving;
    int pad2c;
    signed char dir;
    signed char dist;
    unsigned char pad32;
    unsigned char wide;
};

extern void func_02038c04(struct Track *track);

int func_0203838c(struct Track *track, int index)
{
    signed char delta;
    signed char dir;

    if (index < 0 || track->current == index) {
        return 0;
    }
    if (track->wide == 0) {
        goto low;
    }
    if ((unsigned int)index < track->limit + 1) {
        goto ok;
    }
    return 0;
low:
    if ((unsigned int)index <= track->last - 1) {
        goto ok;
    }
    return 0;
ok:
    func_02038c04(track);
    dir = 1;
    delta = (signed char)(track->current - index);
    track->moving = 1;
    if (delta > 0) {
        track->dist = delta;
    } else if (delta < 0) {
        track->dist = -delta;
        dir = -1;
    } else {
        goto done;
    }
    track->dir = dir;
done:
    return 1;
}
