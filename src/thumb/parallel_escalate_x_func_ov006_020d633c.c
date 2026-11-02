struct Keys {
    unsigned int held;
    unsigned int pressed;
};

struct Entry {
    int count;
    short *value;
};

struct Tables {
    struct Entry *e[2];
};

struct Cell {
    unsigned short x;
    unsigned short y;
};

struct Page {
    char pad0[0x20];
    int mode;
    char pad24;
    unsigned char page;
    unsigned char step;
    char pad27[0xb];
    signed char sel[2];
    signed char next[2];
    signed char count[2];
    signed char rows[2][50];
    int values[2][50];
    int marks[2][50];
    char pad3bc[0x18];
    unsigned short cell_x;
    unsigned short cell_y;
    short shift;
    short offset;
    char pad3dc[4];
    short target;
    char pad3e2[7];
    unsigned char dir;
    unsigned char redraw;
    unsigned char moved;
    char pad3ec[0x24];
    void *button_ok;
    void *button_back;
    void *frame_top;
    void *frame_bottom;
    void *arrow_left;
    void *arrow_right;
    void *hint;
    void *tab_left;
    void *tab_right;
    void *title;
};

extern struct Keys data_020c9d50;
extern const struct Tables data_ov006_020d8a80;
extern void func_ov006_020d74e8(struct Page *p, int side, int row);
extern int func_ov006_020d5d54(void *button);
extern int func_ov006_020d5d64(void *button);
extern int func_ov006_020d5d74(void *button);
extern void func_ov006_020d7fc0(void *button, int value);
extern void *func_0200847c(void);
extern void func_0202a0c0(void *sound, int id);
extern void func_ov006_020d70f0(struct Page *p, void *input);
extern void *func_ov006_020d813c(int a, int b, int c);
extern int func_ov006_020d7320(struct Page *p, int a, signed char index, signed char count, void *left, void *right);
extern void func_ov006_020d4268(void *input);
extern void *func_02010b7c(void);
extern int func_02015188(void *game);
extern int func_0202cb64(void *game);
extern int func_02010b60(void *game);
extern int func_02010b64(void *game);
extern void *func_0201003c(void);
extern struct Cell func_ov006_020d668c(void *board, unsigned short x, unsigned short y);
extern void func_ov006_020d66ac(void *board);
extern void func_0202f64c(void *slot);

void func_ov006_020d633c(struct Page *p, void *input)
{
    struct Tables tables;
    struct Cell cell;
    signed char k;
    int ty;
    int tx;
    signed char cur;
    unsigned char side;
    signed char row;
    signed char now;
    int moved;
    int found;

    side = p->page & 1;
    func_ov006_020d74e8(p, side, p->rows[side][p->sel[side]]);
    if (func_ov006_020d5d54(p->button_ok) != 0) {
        p->step = 5;
        p->page = p->page - 2;
        func_0202a0c0(func_0200847c(), 3);
        func_ov006_020d70f0(p, input);
        p->title = func_ov006_020d813c(0xae, 0, 2);
        return;
    }
    if (func_ov006_020d5d64(p->button_ok) != 0) {
        return;
    }
    if ((data_020c9d50.pressed & 0x2000) != 0 || func_ov006_020d5d74(p->button_ok) != 0) {
        func_ov006_020d7fc0(p->button_ok, 0);
        return;
    }
    if (p->mode == 1) {
        tables = data_ov006_020d8a80;
        row = p->rows[side][p->sel[side]];
        cur = p->marks[side][row];
        moved = func_ov006_020d7320(p, 0, cur, tables.e[side][row].count, p->arrow_left, p->arrow_right);
        now = cur + moved;
        p->values[side][row] = tables.e[side][row].value[now];
        p->marks[side][row] = now;
        if (cur != now) {
            p->shift = p->offset = 0;
            p->target = -1;
            p->step = 1;
            p->dir = 2;
            p->redraw = 1;
            p->moved = moved;
            func_0202a0c0(func_0200847c(), 2);
            func_ov006_020d4268(input);
            return;
        }
    }
    moved = func_ov006_020d7320(p, 1, p->sel[side], p->count[side], p->frame_top, p->frame_bottom);
    if (moved != 0) {
        p->sel[side] = p->next[side] = moved + p->sel[side];
        p->shift = p->offset = 0;
        p->target = -1;
        p->step = 1;
        p->dir = 1;
        p->redraw = 1;
        p->moved = moved;
        func_0202a0c0(func_0200847c(), 2);
        func_ov006_020d4268(input);
        return;
    }
    if (func_02015188(func_02010b7c()) == 0 || func_0202cb64(func_02010b7c()) != 0) {
        return;
    }
    tx = func_02010b60(func_02010b7c());
    ty = func_02010b64(func_02010b7c());
    cell = func_ov006_020d668c(func_0201003c(), tx + 0xfff0, ty + 0xffec);
    if (cell.x == 0xffff || cell.y == 0xffff) {
        return;
    }
    p->cell_x = cell.x;
    p->cell_y = cell.y;
    func_ov006_020d66ac(func_0201003c());
    p->page = p->cell_x + 1;
    p->step = 1;
    p->dir = 0;
    p->redraw = 1;
    p->moved = 1;
    func_0202f64c(&p->hint);
    side = p->page & 1;
    found = -1;
    for (k = 0; k < p->count[side]; k++) {
        if (p->cell_y - 1 == p->rows[side][k]) {
            found = k;
            break;
        }
    }
    if (found == -1) {
        found = 0;
    }
    p->next[side] = p->sel[side] = found;
    p->cell_x = 0xffff;
    p->cell_y = 0xffff;
    p->shift = p->offset = 0;
    p->target = 0;
    func_0202a0c0(func_0200847c(), 2);
    func_ov006_020d4268(input);
}
