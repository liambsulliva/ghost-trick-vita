typedef struct {
    unsigned char on : 1;
} Bits0202a4f4;

typedef struct {
    Bits0202a4f4 bits;
    unsigned char pad;
    unsigned short id;
    int value;
    int zero;
    int handle;
} Rec0202a4f4;

extern int func_0202a4e8(char *object, int key);
extern int func_02006efc(unsigned short id);
extern int func_02029d4c(char *object, int value, int mode);
extern void func_02006fd4(int handle);

int func_0202a4f4(char *object, int index, unsigned short key)
{
    Rec0202a4f4 *rec;

    if (*(unsigned char *)(object + 0x24) == 2) {
        return 0;
    }
    if (*(unsigned short *)(object + 0x28) >= 1) {
        return 0;
    }
    if (func_0202a4e8(object, key) == 0xffff) {
        return 0;
    }
    rec = &((Rec0202a4f4 *)(object + 0x38))[index];
    rec->id = key;
    rec->zero = 0;
    rec->handle = func_02029d4c(object, func_02006efc(rec->id), 1);
    func_02006fd4(rec->handle);
    rec->bits.on = 1;
    *(unsigned short *)(object + 0x28) = *(unsigned short *)(object + 0x28) + 1;
    return 1;
}
