struct Entry {
    short pad[3];
    short a;
    short b;
};

struct Table {
    int name;
    short pad;
    struct Entry e[1];
};

struct Obj {
    char pad[0x158];
    struct Table *table;
    char pad2[4];
    signed char idx;
};

struct Rec {
    char pad[8];
    signed char x;
    signed char y;
    char rest[42];
};

struct Slot {
    struct Rec *recs;
    int pad[3];
};

extern const char data_ov006_020d8930[];
extern const char data_ov006_020d8950[];
extern const char data_ov006_020d8968[];
extern const char data_ov006_020d897c[];
extern struct Slot data_ov006_020dace8[];

extern void func_020593f8(unsigned char *, int, int, int);
extern void func_02073a8c(char *, const char *, ...);
extern int func_02027d4c(char *, int);
extern void func_0205991c(unsigned char *, int, int, int, int);

void func_ov006_020d4968(unsigned char *param_1, int param_2, int param_3)
{
    char buf[0x40];
    struct Obj *obj;

    obj = (struct Obj *)param_1;
    if (*(signed char *)(param_1 + 0x164) == 0x2b && param_1[0x162] == 1 && *(signed char *)(param_1 + 0x160) == 2) {
        func_020593f8(param_1, 0, -1, -1);
        func_02073a8c(buf, data_ov006_020d8930);
    } else if (obj->table->e[obj->idx].b != -1) {
        func_02073a8c(buf, data_ov006_020d8950, obj->table->name, obj->table->e[obj->idx].a, obj->table->e[obj->idx].b);
    } else if (obj->table->e[obj->idx].a != -1) {
        func_02073a8c(buf, data_ov006_020d8968, obj->table->name, obj->table->e[obj->idx].a);
    } else {
        func_02073a8c(buf, data_ov006_020d897c, obj->table->name, obj->table->e[obj->idx].a);
    }
    if (*(signed char *)(param_1 + 0x164) == 0x2b && param_1[0x162] == 1 && *(signed char *)(param_1 + 0x160) < 2) {
        func_020593f8(param_1, **(int **)(param_1 + 0x174),
            data_ov006_020dace8[*(signed char *)(param_1 + 0x164)].recs[param_1[0x162]].x,
            data_ov006_020dace8[*(signed char *)(param_1 + 0x164)].recs[param_1[0x162]].y);
    }
    *(int *)(param_1 + 0x154) = func_02027d4c(buf, 3);
    func_0205991c(param_1, *(int *)(param_1 + 0x154), 0, param_2, param_3);
    *(short *)(param_1 + 0x15e) = 0;
    param_1[0x161] = 1;
}
