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

extern Pair data_020a5470;
extern Pair data_020a5478;
extern Pair data_020a5480;
extern Pair data_020a5488;
extern const char data_02085658;
extern const char data_020856a8;
extern void func_020089c8(Entry *entry, unsigned int owner, unsigned int name, unsigned int ptr, unsigned short tag, unsigned int extra);
extern void func_0200b9b8(Entry *entry, int owner, unsigned short kind, int name, Pair a, Pair b, unsigned short flags, Pair c, Pair d);
extern void func_02008a00();
extern void func_020089fc();

void func_0202fc80(int param_1, void *param_2)
{
    Entry e1;
    Entry e2;

    func_020089c8(&e1, param_1, (unsigned int)&data_02085658, param_1 + 0xc, 1, 0);
    func_02008a00(param_2, &e1);
    func_020089fc(&e1);
    func_0200b9b8(&e2, param_1, 2, (int)&data_020856a8, data_020a5470, data_020a5478, 0x21, data_020a5480, data_020a5488);
    func_02008a00(param_2, &e2);
    func_020089fc(&e2);
}
