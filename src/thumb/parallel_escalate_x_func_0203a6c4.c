struct Menu {
    unsigned char pad0[0x10];
    unsigned char state;
    unsigned char pad11[0xcb];
    unsigned char dir;
    unsigned char sel;
    unsigned char padde[0x12];
    unsigned char busy;
};

void func_0203a6c4(struct Menu *p, int dir)
{
    if (dir == 1) {
        if (p->sel == 11) {
            p->sel = 0;
        } else if (p->sel < 11) {
            p->sel++;
        } else {
            goto done;
        }
    } else if (dir == -1) {
        if (p->sel == 0) {
            p->sel = 11;
        } else if (p->sel > 0) {
            p->sel--;
        } else {
            goto done;
        }
    } else {
        return;
    }
done:
    p->dir = dir;
    p->state = 7;
    p->busy = 0;
}
