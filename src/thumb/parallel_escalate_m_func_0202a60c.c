typedef struct {
    unsigned char on : 1;
} Bits0202a60c;

typedef struct {
    Bits0202a60c bits;
    unsigned char pad;
    unsigned short id;
    int value;
    int first;
    int second;
} Rec0202a60c;

extern void func_020070b0(unsigned short id, int mode);
extern void func_0202a65c(char *object, int value, int flag);

void func_0202a60c(char *object, int index)
{
    Rec0202a60c *rec;

    rec = &((Rec0202a60c *)(object + 0x38))[index];
    if (*(unsigned char *)(object + 0x24) != 2) {
        if (rec->bits.on != 0) {
            func_020070b0(rec->id, 1);
            func_0202a65c(object, rec->second, 1);
            rec->id = 0xffff;
            rec->first = 0;
            rec->second = 0;
            rec->bits.on = 0;
            *(unsigned short *)(object + 0x28) = *(unsigned short *)(object + 0x28) - 1;
        }
    }
}
