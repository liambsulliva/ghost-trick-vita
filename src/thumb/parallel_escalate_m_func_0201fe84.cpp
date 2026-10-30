class Slot0201fe84 {
public:
    virtual ~Slot0201fe84();
    unsigned char pad4;
    unsigned char kind;
    unsigned char low : 7;
    unsigned char dead : 1;
    Slot0201fe84 *next;
};

struct Table0201fe84 {
    int pad0;
    int state;
    int pad8;
    Slot0201fe84 **slots;
};

extern "C" {
extern char *func_02008444(void);
extern Table0201fe84 *func_0201ff10(char *object, int index);
extern void func_0201fa8c(Table0201fe84 *table, int index);
extern void func_0201fab4(Table0201fe84 *table, unsigned int index);

void func_0201fe84(Table0201fe84 *table, unsigned int index)
{
    Slot0201fe84 *slot;
    Table0201fe84 *other;

    slot = table->slots[12];
    if (slot != 0) {
        for (; slot->next != 0; slot = slot->next) {
        }
        if (slot->kind == 4 && table->slots[4] == 0) {
            table->state = 8;
            if (index <= 3) {
                if (table->slots[12] != 0) {
                    delete table->slots[12];
                    table->slots[12] = 0;
                }
                table->state = 7;
                other = func_0201ff10(func_02008444(), 2);
                if (other != 0) {
                    func_0201fa8c(other, 0xc);
                }
                other = func_0201ff10(func_02008444(), 1);
                if (other != 0) {
                    func_0201fa8c(other, 0x29);
                }
            } else if (index == 4) {
                slot = table->slots[12];
                slot->dead = 0;
            }
        }
    }
    func_0201fab4(table, index);
}
}
