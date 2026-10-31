typedef struct {
    int first;
    int second;
} Pair;

typedef struct Entry {
    int name;
    unsigned short kind;
    unsigned short flags;
    int owner;
    Pair a;
    Pair c;
    Pair b;
    Pair d;
    int f2c;
    int f30;
    int f34;
} Entry;

extern Pair data_020a2938;
extern Pair data_020a2ee0;
extern Pair data_020a2ee8;
extern Pair data_020a2ef0;
extern Pair data_020a2ef8;
extern char data_02083bfc;
extern char data_02083c04;
extern void func_0200b9b8(Entry *entry, int owner, unsigned short kind, int name, Pair a, Pair b, unsigned short flags, Pair c, Pair d);
extern void func_02008a00();
extern void func_020089fc();

void func_0200d90c(int param_1, void *param_2)
{
    Entry e1;
    Entry e2;
    Pair z1;
    Pair z2;

    z1 = data_020a2938;
    func_0200b9b8(&e1, param_1, 5, (int)&data_02083bfc, data_020a2ee0, data_020a2ee8, 0, z1, z1);
    func_02008a00(param_2, &e1);
    func_020089fc(&e1);
    z2 = data_020a2938;
    func_0200b9b8(&e2, param_1, 5, (int)&data_02083c04, data_020a2ef0, data_020a2ef8, 0, z2, z2);
    func_02008a00(param_2, &e2);
    func_020089fc(&e2);
}
