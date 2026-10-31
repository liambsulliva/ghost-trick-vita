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
extern Pair data_020abf14;
extern Pair data_020abf1c;
extern char data_0209c12c;
extern char data_0209c154;
extern char data_0209c164;
extern char data_0209c174;
extern void func_0200b9f4(Entry *entry, unsigned int owner, unsigned int name, unsigned int ptr, unsigned short tag, unsigned int extra);
extern void func_02008a64(Entry *entry, unsigned int owner, unsigned int name, unsigned int ptr, unsigned short tag, unsigned int extra);
extern void func_0200b9b8(Entry *entry, int owner, unsigned short kind, int name, Pair a, Pair b, unsigned short flags, Pair c, Pair d);
extern void func_02008a00();
extern void func_020089fc();

void func_0205106c(int param_1, void *param_2)
{
    Entry e1;
    Entry e2;
    Entry e3;
    Entry e4;
    Pair z;

    func_0200b9f4(&e1, param_1, (unsigned int)&data_0209c12c, param_1 + 4, 1, 0);
    func_02008a00(param_2, &e1);
    func_020089fc(&e1);
    func_02008a64(&e2, param_1, (unsigned int)&data_0209c154, param_1 + 0x3c, 1, 0);
    func_02008a00(param_2, &e2);
    func_020089fc(&e2);
    func_02008a64(&e3, param_1, (unsigned int)&data_0209c164, param_1 + 0x44, 1, 0);
    func_02008a00(param_2, &e3);
    func_020089fc(&e3);
    z = data_020a2938;
    func_0200b9b8(&e4, param_1, 5, (int)&data_0209c174, data_020abf14, data_020abf1c, 0, z, z);
    func_02008a00(param_2, &e4);
    func_020089fc(&e4);
}
