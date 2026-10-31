typedef struct {
    unsigned char active : 1;
    unsigned char rest : 7;
    unsigned char state;
    unsigned char count;
    unsigned char pad3[9];
} Slot0202a804;

typedef struct {
    char pad0[0x24];
    unsigned char mode;
    char pad25[0x1bf];
    Slot0202a804 slots[8];
} Obj0202a804;

extern int func_0202b014(void *object, unsigned char index);
extern void func_0202aba4(void *object, unsigned char index, int arg);

void func_0202a804(char *param_1)
{
    Slot0202a804 *e;
    int result;
    int i;

    if (*(unsigned char *)(param_1 + 0x24) != 1) {
        for (i = 0; i < 8; i++) {
            e = (Slot0202a804 *)(param_1 + 0x1e4) + i;
            if (e->active) {
                result = func_0202b014(param_1, i);
                if (result == 0 && e->state != 6) {
                    e->state = 0;
                    e->active = 0;
                    e->count = 0;
                } else {
                    switch (e->state) {
                    case 6:
                        if (result != 0) {
                            e->state = result;
                        }
                        break;
                    case 3:
                        if (result == 1) {
                            func_0202aba4(param_1, i, 0);
                        }
                        break;
                    case 2:
                    case 4:
                        if (result == 1) {
                            e->state = result;
                        }
                        break;
                    }
                }
            }
        }
    }
}
