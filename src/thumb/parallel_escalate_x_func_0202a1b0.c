typedef struct {
    unsigned char on : 1;
} Bits0202a1b0;

typedef struct {
    Bits0202a1b0 bits;
    unsigned char state;
    unsigned char extra;
    unsigned char pad;
    int value;
    int key;
} Rec0202a1b0;

extern char data_02080e84;
extern char data_02080e86;
extern void func_02007300(int a, int b, int c, int flag);

void func_0202a1b0(char *object, int index, int flag)
{
    int i;

    if (*(unsigned char *)(object + 0x24) == 1) {
        return;
    }
    func_02007300(-1, *(short *)(&data_02080e86 + index * 8), *(short *)(&data_02080e84 + index * 8), flag);
    for (i = 0; i < 8; i++) {
        if (index == ((Rec0202a1b0 *)(object + 0x1e4))[i].key) {
            if (flag != 0) {
                ((Rec0202a1b0 *)(object + 0x1e4))[i].state = 3;
            } else {
                ((Rec0202a1b0 *)(object + 0x1e4))[i].bits.on = 0;
                ((Rec0202a1b0 *)(object + 0x1e4))[i].state = 0;
                ((Rec0202a1b0 *)(object + 0x1e4))[i].extra = 0;
            }
        }
    }
}
