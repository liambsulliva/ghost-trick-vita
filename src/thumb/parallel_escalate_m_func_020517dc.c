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
extern Pair data_020abf24;
extern Pair data_020abf2c;
extern char data_0209c12c;
extern char data_0209c1a4;
extern char data_0209c138;
extern char data_0209c1ac;
extern char data_0209c1b4;
extern char data_0209c1bc;
extern char data_0209c174;
extern void func_0202f6a0(int object, void *archive);
extern void func_0200b9f4(Entry *entry, unsigned int owner, unsigned int name, unsigned int ptr, unsigned short tag, unsigned int extra);
extern void func_02008a64(Entry *entry, unsigned int owner, unsigned int name, unsigned int ptr, unsigned short tag, unsigned int extra);
extern void func_020121e4(Entry *entry, unsigned int owner, unsigned int name, unsigned int ptr, unsigned short tag, unsigned int extra);
extern void func_0200b9b8(Entry *entry, int owner, unsigned short kind, int name, Pair a, Pair b, unsigned short flags, Pair c, Pair d);
extern void func_02008a00();
extern void func_020089fc();

void func_020517dc(int param_1, void *param_2)
{
    Entry e1;
    Entry e2;
    Entry e3;
    Entry e4;
    Entry e5;
    Entry e6;
    Entry e7;
    Pair z;

    func_0202f6a0(param_1, param_2);
    func_0200b9f4(&e1, param_1, (unsigned int)&data_0209c12c, param_1 + 0x28, 0, 0);
    func_02008a00(param_2, &e1);
    func_020089fc(&e1);
    func_0200b9f4(&e2, param_1, (unsigned int)&data_0209c1a4, param_1 + 0x60, 0, 0);
    func_02008a00(param_2, &e2);
    func_020089fc(&e2);
    func_02008a64(&e3, param_1, (unsigned int)&data_0209c138, param_1 + 0xb0, 1, 0);
    func_02008a00(param_2, &e3);
    func_020089fc(&e3);
    func_02008a64(&e4, param_1, (unsigned int)&data_0209c1ac, param_1 + 0xb4, 1, 0);
    func_02008a00(param_2, &e4);
    func_020089fc(&e4);
    func_020121e4(&e5, param_1, (unsigned int)&data_0209c1b4, param_1 + 0x100, 0, 0);
    func_02008a00(param_2, &e5);
    func_020089fc(&e5);
    func_020121e4(&e6, param_1, (unsigned int)&data_0209c1bc, param_1 + 0x101, 0, 0);
    func_02008a00(param_2, &e6);
    func_020089fc(&e6);
    z = data_020a2938;
    func_0200b9b8(&e7, param_1, 5, (int)&data_0209c174, data_020abf24, data_020abf2c, 0, z, z);
    func_02008a00(param_2, &e7);
    func_020089fc(&e7);
}
