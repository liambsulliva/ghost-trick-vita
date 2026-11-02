typedef struct Heap1ad80 Heap1ad80;

typedef struct HeapVt1ad80 {
    void *s[2];
    void *(*alloc)(Heap1ad80 *h, unsigned int size, char *name);
} HeapVt1ad80;

struct Heap1ad80 {
    HeapVt1ad80 *vt;
};

class Text1ad80 {
public:
    virtual void s0();
    virtual void s1();
    virtual void s2();
    virtual void s3();
    virtual void s4();
    virtual void s5();
    virtual void s6();
    virtual void s7();
    virtual void s8(int a);
    virtual void s9();
    virtual void s10();
    virtual void s11();
    virtual void s12();
    virtual void s13();
    virtual void s14();
    virtual void s15();
    virtual void s16();
    virtual void s17();
    virtual void s18();
    virtual void s19();
    virtual void s20();
    virtual void s21();
    virtual void s22();
    virtual void s23();
    virtual void s24();
    virtual void s25();
    virtual void s26();
    virtual void s27();
    virtual void s28();
    virtual void s29();
    virtual void s30();
    virtual void s31();
    virtual void s32();
    virtual void s33();
    virtual void s34();
    virtual void s35();
    virtual void s36();
    virtual void s37();
    virtual void s38();
    virtual void s39();
    virtual void s40();
    virtual void s41();
    virtual void s42();
    virtual void s43();
    virtual void s44();
    virtual void s45();
    virtual void s46();
    virtual void s47();
    virtual void s48();
    virtual void s49();
    virtual void s50();
    virtual void s51();
    virtual void s52();
    virtual void s53();
    virtual void s54();
    virtual void s55();
    virtual void s56();
    virtual void s57();
    virtual void s58();
    virtual void s59(unsigned short *text);
    virtual void s60();
    virtual void s61();
    virtual void s62();
    virtual void s63();
    virtual void s64();
    virtual void s65();
    virtual void s66();
    virtual void s67();
    virtual void s68();
    virtual void s69();
    virtual void s70();
    virtual void s71();
    virtual void s72();
    virtual void s73();
    virtual void s74();
    virtual void s75();
    virtual void s76();
    virtual void s77();
    virtual void s78();
    virtual void s79();
    virtual void s80();
    virtual void s81();
    virtual void s82();
    virtual void s83();
    virtual void s84();
    virtual void s85();
    virtual void s86();
    virtual void s87();
    virtual void s88();
    virtual void s89();
    virtual void s90();
    virtual void s91();
    virtual void s92(int a, int b);
    void *cells;
    int cell_count;
    char padc[8];
    unsigned short *text;
    unsigned short *start;
    char pad1c[4];
    unsigned short pos;
    unsigned char visible;
    unsigned char dirty;
    char pad24[2];
    unsigned short state;
    char pad28[8];
    short x;
    short y;
    char pad34[0xc];
    unsigned short line;
    unsigned short col;
    unsigned short row;
    unsigned short width;
    unsigned short height;
    unsigned short pitch;
    unsigned short span;
    char pad4e[0x12];
    int font;
    char pad64[4];
    void *front;
    void *back;
    unsigned int size;
    unsigned int half;
};

extern "C" {
extern char data_02084cd8[];
extern char data_020b9df4[];
extern void func_0201778c(Text1ad80 *t);
extern void func_02017714(Text1ad80 *t, int a);
extern void func_0201767c(Text1ad80 *t);
extern void *func_02007bd8(void);
extern Heap1ad80 *func_02007b70(void *a);
extern void func_0201aeb0(void *dst, int value, unsigned int size);
extern void func_0201b134(Text1ad80 *t);
extern void func_0201cf2c(char *a, int font);
extern void func_02016fbc(char *a, int b);
void func_0201ad80(Text1ad80 *t, unsigned short *text, short x, short y);
}

void func_0201ad80(Text1ad80 *t, unsigned short *text, short x, short y)
{
    Heap1ad80 *heap;

    t->pos = 0;
    func_0201778c(t);
    t->dirty = 1;
    t->visible = 1;
    t->start = text;
    t->text = text;
    t->s8(1);
    t->s92(t->visible, 9);
    func_02017714(t, 2);
    t->col = 0;
    t->row = 0;
    t->line = 0;
    t->width = 0x100;
    t->height = 0x20;
    t->pitch = t->width;
    t->span = t->width;
    t->x = x;
    t->y = y;
    t->state = 2;
    func_0201767c(t);
    t->size = t->width * t->height;
    t->half = t->size >> 1;
    if (t->front == 0) {
        heap = func_02007b70(func_02007bd8());
        t->front = heap->vt->alloc(heap, t->size, data_02084cd8);
    }
    if (t->back == 0) {
        heap = func_02007b70(func_02007bd8());
        t->back = heap->vt->alloc(heap, t->size, data_02084cd8);
    }
    t->cell_count = 0x12;
    if (t->cells == 0) {
        heap = func_02007b70(func_02007bd8());
        t->cells = heap->vt->alloc(heap, t->cell_count * 8, data_02084cd8);
        func_0201aeb0(t->cells, 0x2c0, t->cell_count * 8);
    }
    func_0201b134(t);
    func_0201cf2c(data_020b9df4, t->font);
    func_02016fbc(data_020b9df4, 3);
    t->s59(t->text);
}
