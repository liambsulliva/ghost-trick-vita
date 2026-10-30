class Table {
public:
    virtual void slot0();
    virtual void slot1();
    virtual void slot2();
    virtual void slot3();
    virtual void slot4();
    virtual unsigned short *Name(int index);
};
extern "C" {
extern void *func_0200883c(void);
extern void *func_02008824(void *system);
extern int func_0200cf3c(void *field);
extern int func_0200cd70(Table *table,unsigned int index);
extern void func_0200cd54(Table *table,unsigned int index);
extern int func_0200cc5c(Table *table,unsigned short *key,int a,unsigned short b,int c);
int func_0200cecc(Table *table,unsigned short *key,int a,unsigned short b,int c)
{
    unsigned short *name;
    unsigned int j;
    unsigned int i;
    if (func_0200cf3c(func_02008824(func_0200883c())) != 0) {
        for (i = 0; i < 16; i++) {
            if (func_0200cd70(table,i) != 0) {
                name = table->Name(i);
                for (j = 1; j < 16; j++) {
                    if (key[j] != name[j]) {
                        break;
                    }
                }
                if (j == 16) {
                    func_0200cd54(table,i);
                    return i;
                }
            }
        }
    }
    return func_0200cc5c(table,key,a,b,c);
}
}
