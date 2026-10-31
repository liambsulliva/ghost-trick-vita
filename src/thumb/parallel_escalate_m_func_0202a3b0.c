typedef struct {
    unsigned char on : 1;
    unsigned char active : 1;
    unsigned char paused : 1;
} Bits0202a3b0;

typedef struct {
    Bits0202a3b0 bits;
    unsigned char state;
    unsigned short id;
    int value;
    int first;
    int second;
} Rec0202a3b0;

extern int func_0202a7bc(char *object, unsigned short id);
extern int func_0202a440(char *object, int index);
extern int func_02077118(int value);
extern void func_0202a714(char *object, int index, int flag);

void func_0202a3b0(char *object)
{
    Rec0202a3b0 *rec;
    int result;

    if (*(unsigned char *)(object + 0x24) != 2) {
        rec = (Rec0202a3b0 *)(object + 0x38);
        if (rec->bits.active != 0) {
            result = func_0202a7bc(object, rec->id);
            rec->first = func_02077118(func_0202a440(object, 0));
            if (result == 0 && rec->state != 6) {
                rec->state = 0;
                rec->bits.active = 0;
                rec->bits.paused = 0;
            } else {
                switch (rec->state) {
                case 6:
                    if (result != 0) {
                        rec->state = result;
                    }
                    break;
                case 3:
                    if (result == 1) {
                        func_0202a714(object, 0, 0);
                    }
                    break;
                case 2:
                case 4:
                    if (result == 1) {
                        rec->state = result;
                    }
                    break;
                }
            }
        }
    }
}
