struct Item {
    short v[7];
};

struct Page {
    char pad0[0x24];
    unsigned char state;
    unsigned char page;
    char pad26[0x3de];
    unsigned char touch;
    char pad405[3];
    char pen[8];
    void *button_ok;
    void *button_back;
    void *frame_top;
    void *frame_bottom;
    void *arrow_left;
    void *arrow_right;
    char pad428[0x234];
    struct Item *items;
};

extern char data_020cd894[];
extern void func_ov006_020d5114(struct Page *p);
extern void *func_02022df0(struct Page *p);
extern void *func_020082f8(void *manager, char *name);
extern unsigned char func_0200acc8(char *pen);
extern void func_ov006_020d4ff0(void *button, unsigned char *touch);
extern void func_ov006_020d545c(struct Page *p, void *info);
extern void func_ov006_020d56f4(struct Page *p, void *info);
extern void func_ov006_020d5788(struct Page *p, void *info);
extern void func_ov006_020d5828(struct Page *p, void *info);
extern void func_ov006_020d583c(struct Page *p);
extern void func_ov006_020d5870(struct Page *p, void *info);
extern void func_ov006_020d58a8(struct Page *p, void *info);
extern void func_ov006_020d58e4(struct Page *p);
extern void func_ov006_020d5928(struct Page *p);
extern void func_ov006_020d598c(struct Page *p, void *info);
extern void func_ov006_020d4ffc(void *object);
extern void func_ov006_020d7ddc(struct Item *item);
extern void func_ov006_020d5a28(struct Page *p, void *info);
extern void func_ov006_020d5a88(struct Page *p, void *info);

void func_ov006_020d4e5c(struct Page *p)
{
    void *manager;
    void *info;
    unsigned int i;

    if (p->state == 0) {
        p->state = 1;
        return;
    }
    if (p->state == 1) {
        func_ov006_020d5114(p);
        return;
    }
    manager = func_02022df0(p);
    if (manager == 0) {
        return;
    }
    info = func_020082f8(manager, data_020cd894);
    if (info == 0) {
        return;
    }
    p->touch = func_0200acc8(p->pen);
    if (p->button_ok != 0) {
        func_ov006_020d4ff0(p->button_ok, &p->touch);
    }
    if (p->button_back != 0) {
        func_ov006_020d4ff0(p->button_back, &p->touch);
    }
    switch (p->state) {
    case 2:
        func_ov006_020d545c(p, info);
        return;
    case 3:
        func_ov006_020d56f4(p, info);
        return;
    case 4:
        func_ov006_020d5788(p, info);
        return;
    case 5:
        func_ov006_020d5828(p, info);
        return;
    case 6:
        func_ov006_020d583c(p);
        return;
    case 8:
        func_ov006_020d5870(p, info);
        return;
    case 9:
        func_ov006_020d58a8(p, info);
        return;
    case 10:
        func_ov006_020d58e4(p);
        return;
    case 11:
        func_ov006_020d5928(p);
        return;
    case 12:
        func_ov006_020d598c(p, info);
        return;
    }
    if (p->button_ok != 0) {
        func_ov006_020d4ffc(p->button_ok);
    }
    if (p->button_back != 0) {
        func_ov006_020d4ffc(p->button_back);
    }
    if (p->frame_top != 0) {
        func_ov006_020d4ffc(p->frame_top);
    }
    if (p->frame_bottom != 0) {
        func_ov006_020d4ffc(p->frame_bottom);
    }
    if (p->arrow_left != 0) {
        func_ov006_020d4ffc(p->arrow_left);
    }
    if (p->arrow_right != 0) {
        func_ov006_020d4ffc(p->arrow_right);
    }
    if (p->items != 0) {
        for (i = 0; i < 2; i++) {
            func_ov006_020d7ddc(&p->items[i]);
        }
    }
    if (p->page < 2) {
        func_ov006_020d5a28(p, info);
    } else {
        func_ov006_020d5a88(p, info);
    }
}
