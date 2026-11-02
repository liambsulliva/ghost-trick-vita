typedef struct Slide38d0c {
    char pad0[0x10];
    unsigned char state;
    char pad11[0xa3];
    int pos;
    char padb8[9];
    unsigned char timer;
    char padc2[6];
    int home;
} Slide38d0c;

void func_02038d0c(Slide38d0c *p)
{
    if (p->state == 0) {
        p->pos += 0x28000;
        p->state = 1;
        p->timer = 5;
        return;
    }
    if (p->state == 1) {
        return;
    }
    if (p->state == 2 || p->state == 3) {
        if (p->timer != 0) {
            p->timer--;
            return;
        }
        if (p->pos > p->home) {
            p->pos -= 0x3000;
        } else {
            p->pos = p->home;
            p->state = 1;
        }
    }
    if (p->state == 4) {
        if (p->pos < p->home + 0x28000) {
            p->pos += 0x3000;
            return;
        }
        p->pos = p->home + 0x28000;
        p->state = 6;
    }
}
