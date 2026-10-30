typedef struct {
    unsigned char on : 1;
    unsigned char active : 1;
    unsigned char paused : 1;
} Bits0202a714;

typedef struct {
    Bits0202a714 bits;
    unsigned char state;
    short id;
    int value;
    int first;
    int second;
} Rec0202a714;

extern void func_020072ac(int a, int id, int flag);

void func_0202a714(char *object, int index, int flag)
{
    Rec0202a714 *rec;

    rec = &((Rec0202a714 *)(object + 0x38))[index];
    if (*(unsigned char *)(object + 0x24) != 2) {
        if (rec->bits.active != 0) {
            if (flag != 0 && rec->bits.paused == 0) {
                func_020072ac(-1, rec->id, flag);
                rec->state = 3;
            } else {
            func_020072ac(-1, rec->id, 0);
            rec->bits.active = 0;
            rec->bits.paused = 0;
            rec->state = 0;
            }
        }
    }
}
