class Table {
public:
    virtual void v00();
    virtual void v04();
    virtual void v08();
    virtual void v0c();
    virtual void v10();
    virtual void v14();
    virtual int v18(unsigned int index);
};

struct Owner {
    char pad[0x14];
    Table *table;
};

extern "C" {
int func_0201f944(int object, unsigned int index);
unsigned int func_02037170(unsigned int param_1);

int func_0203853c(struct Owner *owner)
{
    int best;
    unsigned int i;
    int value;

    best = -1;
    for (i = 0; i < 0x18; i++) {
        if (func_0201f944((int)owner->table, i) >= 0 && func_02037170(i) == 0) {
            value = owner->table->v18(i);
            if (best < value) {
                best = (short)value;
            }
        }
    }
    return best;
}
}
