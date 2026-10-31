class Holder {
public:
    virtual ~Holder();
};

class Target {
public:
    virtual void slot0();
    virtual void slot1();
    virtual void slot2();
    virtual void slot3();
    virtual void slot4();
    virtual void slot5();
    virtual void Attach(int key,void *item,int value);
};

struct Entry {
    int first;
    int second;
    int key;
    unsigned char low : 2;
    unsigned char big : 1;
    unsigned char flag : 1;
    unsigned char kind : 4;
    unsigned char state;
    unsigned short id;
    int value;
    Target *target;
    int word18;
    unsigned int word1c;
    int word20;
    void *word24;
};

extern "C" {
extern void *func_0200883c(void);
extern void *func_02008824(void *object);
extern unsigned int func_0201e97c(void *pack,int block,unsigned int index);
extern unsigned char *func_02008860(void);
extern Target *func_0200affc(unsigned char *object,unsigned int index);
extern Target *func_0200b00c(unsigned char *object,unsigned int index);
extern int func_0200cb04(Target *target,unsigned int id,unsigned int value);
extern void *func_02031254(unsigned int size);
extern Holder *func_0200a378(void *object);

int func_0200af44(struct Entry *entry,unsigned int id,unsigned int kind)
{
    int key;
    Holder *holder;

    if (func_0201e97c(func_02008824(func_0200883c()),0,id) > 0x20) {
        entry->target = func_0200affc(func_02008860(),kind);
        entry->big = 1;
    }
    else {
        entry->target = func_0200b00c(func_02008860(),kind);
        entry->big = 0;
    }
    entry->kind = (unsigned char)kind;
    entry->id = id;
    key = func_0200cb04(entry->target,id,entry->word1c);
    entry->key = key;
    if (key != -1) {
        if (entry->word24 != 0) {
            entry->target->Attach(key,entry->word24,0x20);
        }
        else {
            holder = (Holder *)func_02031254(4);
            if (holder != 0) {
                holder = func_0200a378(holder);
            }
            if (holder != 0) {
                entry->target->Attach(entry->key,holder,entry->word20 >> 4);
                delete holder;
            }
        }
    }
    return entry->key;
}
}
