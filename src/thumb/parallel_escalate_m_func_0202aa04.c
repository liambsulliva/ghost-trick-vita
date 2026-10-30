typedef struct {
    unsigned char on : 1;
} Bits0202aa04;

typedef struct {
    Bits0202aa04 bits;
    unsigned char pad;
    unsigned short id;
    int value;
    int handle;
} Rec0202aa04;

extern unsigned short func_0202aa58(char *object, int value);
extern int func_02006f5c(unsigned short id, int mode);
extern int func_02029d4c(char *object, int value, int mode);
extern void func_02007038(unsigned short id, int handle, int mode);

void func_0202aa04(char *object, int index, int value)
{
    Rec0202aa04 *rec;

    if (*(unsigned char *)(object + 0x24) != 1) {
    rec = &((Rec0202aa04 *)(object + 0x48))[index];
    rec->id = func_0202aa58(object, value);
    rec->handle = func_02029d4c(object, func_02006f5c(rec->id, 6), 2);
    func_02007038(rec->id, rec->handle, 6);
    rec->bits.on = 1;
    *(unsigned short *)(object + 0x2a) = *(unsigned short *)(object + 0x2a) + 1;
    }
}
