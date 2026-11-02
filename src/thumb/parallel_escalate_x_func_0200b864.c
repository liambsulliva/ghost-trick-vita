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
extern Pair data_020a2d20;
extern Pair data_020a2d28;
extern Pair data_020a2d30;
extern Pair data_020a2d38;
extern char data_02083b58;
extern char data_02083b64;
extern char data_02083b70;
extern char data_02083b7c;
extern char data_02083b84;
extern char data_02083b90;
extern int func_020089c8();
extern void func_0200b9b8(Entry *entry, int owner, unsigned short kind, int name, Pair a, Pair b, unsigned short flags, Pair c, Pair d);
extern int func_0200b9f4();
extern int func_0200b374();
extern void func_02008a00();
extern void func_020089fc();

void func_0200b864(int param_1, void *param_2)
{
    Entry e1;
    Entry e2;
    Entry e3;
    Entry e4;
    Entry e5;
    Entry e6;
    Pair z1;
    Pair z2;

    func_020089c8(&e1, param_1, (int)&data_02083b58, param_1 + 0xc, 0, 0);
    func_02008a00(param_2, &e1);
    func_020089fc(&e1);
    func_020089c8(&e2, param_1, (int)&data_02083b64, param_1 + 0x10, 0, 0);
    func_02008a00(param_2, &e2);
    func_020089fc(&e2);
    z1 = data_020a2938;
    func_0200b9b8(&e3, param_1, 4, (int)&data_02083b70, data_020a2d20, data_020a2d28, 1, z1, z1);
    func_02008a00(param_2, &e3);
    func_020089fc(&e3);
    z2 = data_020a2938;
    func_0200b9b8(&e4, param_1, 5, (int)&data_02083b7c, data_020a2d30, data_020a2d38, 0, z2, z2);
    func_02008a00(param_2, &e4);
    func_020089fc(&e4);
    func_0200b9f4(&e5, param_1, (int)&data_02083b84, param_1 + 0x14, 0, 0);
    func_02008a00(param_2, &e5);
    func_020089fc(&e5);
    func_0200b374(&e6, param_1, (int)&data_02083b90, param_1 + 100, 0, 0);
    func_02008a00(param_2, &e6);
    func_020089fc(&e6);
}
