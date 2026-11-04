struct Panel;

struct Row18 {
    int v[18];
};

extern struct Row18 data_ov003_020d5a98;

int func_ov003_020d50b8(struct Panel *panel, int n)
{
    struct Row18 t;

    t = data_ov003_020d5a98;
    if (n < 1) {
        n = 1;
    }
    if (n >= 19) {
        n = 18;
    }
    return (t.v - 1)[n];
}
