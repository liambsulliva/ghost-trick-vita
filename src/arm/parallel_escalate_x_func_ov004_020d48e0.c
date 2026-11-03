typedef struct Reader48e0 {
    unsigned char *pos;
    char pad4[0x3b0];
    int value;
} Reader48e0;

typedef struct Stream48e0 {
    char pad0[8];
    signed char tag0;
    signed char tag1;
    char pada[0x2a];
    Reader48e0 *reader;
    int (*parse)(Reader48e0 *r);
    char pad3c[0x28];
    int *values;
    char pad68[0x34];
    unsigned int limit;
    unsigned int count;
    int flag;
    int wrap;
    int ring[6];
    char padc4[8];
    int markc;
    int markd;
} Stream48e0;

int func_ov004_020d48e0(Stream48e0 *s)
{
    int skip;
    int i;

    if (s->limit > s->count) {
        return 0;
    }
    if (*(unsigned short *)s->reader->pos & 0x8000) {
        s->flag = 1;
        skip = 4;
    } else {
        skip = 0;
        s->flag = 0;
    }
    s->reader->pos += s->parse(s->reader);
    if (s->tag0 == 'N' && s->tag1 == '3') {
        s->reader->pos += skip;
    }
    s->values[s->ring[0]] = s->reader->value;
    s->markc = 0;
    s->markd = 0;
    for (i = 0; i < 6; i++) {
        s->ring[i]++;
        if (s->wrap == s->ring[i]) {
            s->ring[i] = 0;
        }
    }
    s->count++;
    return 1;
}
