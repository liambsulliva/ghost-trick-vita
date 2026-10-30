typedef struct {
    unsigned char on : 1;
    unsigned char held : 1;
    unsigned char pad;
    unsigned short id;
    int value;
    int handle;
} Track0202aab4;

typedef struct {
    int a;
    int b;
    int key;
} Rec0202aab4;

typedef struct {
    char pad[0x24];
    unsigned char mode;
    char pad25[5];
    unsigned short count;
    char pad2c[0x1c];
    Track0202aab4 tracks[0x1c];
    char pad198[0x1e4 - 0x198];
    Rec0202aab4 recs[8];
} Owner0202aab4;

extern unsigned short func_0202aa58(Owner0202aab4 *owner, int key);
extern void func_0202aba4(Owner0202aab4 *owner, unsigned char slot, int flag);
extern void func_020070b0(unsigned short id, int mode);
extern void func_0202a65c(Owner0202aab4 *owner, int value, int flag);

void func_0202aab4(Owner0202aab4 *owner, int index)
{
    Track0202aab4 *track;
    int i;

    track = &owner->tracks[index];
    if (owner->mode != 1 && track->on) {
        for (i = 0; i < 8; i++) {
            if (track->id == func_0202aa58(owner, owner->recs[i].key)) {
                func_0202aba4(owner, i, 0);
            }
        }
        func_020070b0(track->id, 6);
        func_0202a65c(owner, track->handle, 2);
        track->id = 0xffff;
        track->handle = 0;
        track->on = 0;
        track->held = 0;
        owner->count--;
    }
}
