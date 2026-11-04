typedef struct Info77150 {
    unsigned short prg;
    unsigned char f2;
    unsigned char f3;
    unsigned char f4;
    unsigned char f5;
    unsigned char f6;
    unsigned char f7;
    char pad8;
    unsigned char count;
    unsigned char list[1];
} Info77150;

typedef struct Chan77150 {
    unsigned char id;
    char pad1[0x4f];
    struct Chan77150 *next;
} Chan77150;

typedef struct Track77150 {
    char pad0[2];
    unsigned short prg;
    unsigned char f4;
    unsigned char f5;
    signed char f6;
    unsigned char f7;
    signed char f8;
    char pad9[0xa];
    signed char f13;
    char pad14[0x28];
    Chan77150 *chan;
} Track77150;

typedef struct Player77150 {
    char pad0[8];
    unsigned char tracks[16];
    char pad18[0xc];
} Player77150;

typedef struct Shared77150 {
    char pad0[0x540];
    Player77150 players[16];
    Track77150 tracks[32];
    char pad[0x11c0 - 0x780 - 32 * 0x40];
    int work;
} Shared77150;

int func_02077150(Shared77150 *shared, int player, int track, Info77150 *info)
{
    unsigned char id;
    Track77150 *t;
    Chan77150 *c;
    Chan77150 *p;
    int w;

    if (player < 0 || player > 15) {
        return 0;
    }
    if (track < 0 || track > 15) {
        return 0;
    }
    id = shared->players[player].tracks[track];
    if (id == 0xff) {
        return 0;
    }
    t = &shared->tracks[id];
    info->prg = t->prg;
    info->f2 = t->f4;
    info->f3 = t->f5;
    info->f4 = t->f6;
    info->f5 = t->f7;
    info->f6 = t->f8 + 0x40;
    info->f7 = t->f13;
    w = shared->work;
    p = t->chan;
    if (p == 0) {
        c = 0;
    } else {
        c = (Chan77150 *)((char *)shared + ((int)p - w));
    }
    info->count = 0;
    if (c != 0) {
        do {
            info->list[info->count] = c->id;
            info->count++;
            w = shared->work;
            p = c->next;
            if (p == 0) {
                c = 0;
            } else {
                c = (Chan77150 *)((char *)shared + ((int)p - w));
            }
        } while (c != 0);
    }
    return 1;
}
