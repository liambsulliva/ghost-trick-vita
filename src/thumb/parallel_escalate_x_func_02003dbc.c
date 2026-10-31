struct State {
    unsigned int before[4];
    int data;
};
typedef struct {
    short a;
    short b;
    void *ptr;
    char pad8[10];
    unsigned char c;
    char pad13[0x19];
    int result;
    char pad30[0x1c];
} Entry02003dbc;
extern struct State data_020b18a0;

int func_02003dbc(void *param_1, int param_2, int param_3, unsigned int param_4)
{
    Entry02003dbc *e;
    int base;
    int i;
    short sx;
    short sy;

    i = 0;
    base = data_020b18a0.data;
    sy = param_3;
    sx = param_2;
    for (; i < 0x11; i++) {
        if (i == 0x10) {
            break;
        }
        e = (Entry02003dbc *)(base + 0x15bc) + i;
        if (e->ptr == param_1 && e->a == sx && (param_3 == -1 || e->b == sy) &&
            (e->c == param_4 || param_4 == 0xff)) {
            break;
        }
    }
    if (i == 0x10) {
        return -1;
    }
    return e->result;
}
