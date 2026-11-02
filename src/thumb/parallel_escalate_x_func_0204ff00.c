struct Turn {
    char pad0[0x2a];
    signed char next;
    signed char current;
    signed char target;
    char pad2d[0x17];
    unsigned int mask;
    void *parts[12];
};

extern unsigned char data_0209be60[][6];
extern int func_0204faf4(void *part);
extern void *func_02008444(void);
extern int func_0201008c(void *system);
extern int func_020100bc(void *system);
extern int func_02050054(void *part);
extern int func_0201132c(void *system);
extern void func_0204fb80(struct Turn *turn, int a, int b);

void func_0204ff00(struct Turn *turn, int dir)
{
    int full;
    unsigned char prev;
    int next;
    int found;
    int i;
    int side;

    full = 0;
    prev = turn->current;
    if (prev <= 2) {
        full = 1;
    }
    next = dir + 3;
    if (next == turn->current) {
        if (turn->current == 3) {
            if (func_0204faf4(turn->parts[0]) == 1 || func_0204faf4(turn->parts[0]) == 2 || func_0201008c(func_02008444()) != 0 || func_0204faf4(turn->parts[4]) == 1 || func_0204faf4(turn->parts[4]) == 2 || func_0204faf4(turn->parts[4]) == 4 || func_020100bc(func_02008444()) != 0) {
                return;
            }
        } else if (turn->current == 4) {
            if (func_0204faf4(turn->parts[0]) == 1 || func_0204faf4(turn->parts[0]) == 2) {
                return;
            }
        }
    }
    turn->next = next;
    turn->current = turn->next;
    turn->target = 0;
    found = 0;
    for (i = 0; i < 12; i++) {
        if ((i == 1 || i == 5) && turn->current != 7) {
            if (turn->parts[i] == 0) {
                continue;
            }
            side = func_02050054(turn->parts[i]);
            if ((func_0201132c(func_02008444()) ^ side) == 0) {
                continue;
            }
        }
        if (func_0204faf4(turn->parts[i]) == 1 || func_0204faf4(turn->parts[i]) == 2) {
            found = 1;
            break;
        }
    }
    turn->mask = 0;
    if (found) {
        func_0204fb80(turn, -1, -1);
        turn->target = next;
        if (full != 0 || prev == turn->target) {
            for (i = 0; i < 6; i++) {
                turn->mask |= 1 << data_0209be60[turn->target - 3][i];
            }
        }
    }
}
