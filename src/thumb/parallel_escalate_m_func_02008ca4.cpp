class Manager {
public:
    virtual void slot0();
    virtual void slot1();
    virtual void *Alloc(unsigned int size,void *tag);
    virtual void slot3();
    virtual void Free(void *block);
};

class Reader {
public:
    virtual ~Reader();
};

struct Entry {
    unsigned int count;
    unsigned int size;
    unsigned int magic;
    int time;
    unsigned char valid;
    unsigned char kind;
    unsigned char pad[0xe];
};

struct Loader {
    unsigned char head[6];
    unsigned char status;
    signed char index;
    unsigned char bank;
    unsigned char pad9[3];
    unsigned char *buffer;
    unsigned char pad10[0xc];
    unsigned char work[0x400];
    unsigned char thread[0xc0];
    unsigned char stack[0x1000];
    unsigned int tail;
    struct Entry slots[4][1];
};

struct Bank {
    unsigned int size;
    unsigned int extra;
};

extern "C" {
extern struct Bank data_02083a90[];
extern char data_02083a94;
extern char data_020b4280;
extern void func_0200965c(struct Loader *loader,int mode);
extern void *func_02007bd8(void);
extern Manager *func_02007b70(void *system);
extern void func_02008ea4(void *object,int value);
extern void *func_02008eb0(void);
extern int func_01ff80b8(void *object);
extern Reader *func_02008b34(void *heap);
extern int func_02008b78(Reader *reader,void *first,void *second,unsigned int third);
extern void func_0207303c(void *src,void *dst,unsigned int size);
extern void func_02073ed4(void *thread,void (*proc)(void),void *arg,void *stack,unsigned int stackSize,unsigned int priority);
extern void func_02074188(void *thread);
extern void func_02008c98(void);

int func_02008ca4(struct Loader *loader,void *src,unsigned int size,int bank)
{
    unsigned int base;
    unsigned int best;
    unsigned int found;
    int i;
    Manager *manager;
    Reader *reader;

    func_0200965c(loader,1);
    base = data_02083a90[bank].size;
    if (size > base - 0x20) {
        loader->status = 2;
        return 2;
    }
    loader->status = 0;
    if (loader->buffer != 0) {
        if (func_02007b70(func_02007bd8()) != 0) {
            manager = func_02007b70(func_02007bd8());
            manager->Free(loader->buffer);
            loader->buffer = 0;
        }
    }
    manager = func_02007b70(func_02007bd8());
    loader->buffer = (unsigned char *)manager->Alloc(base,&data_02083a94);
    func_02008ea4(loader->buffer,base);
    found = 0;
    loader->bank = bank;
    loader->index = 0;
    best = 0xffffffff;
    for (i = 0; i < 1; i++) {
        if (loader->slots[loader->bank][i].count < best) {
            best = loader->slots[loader->bank][i].count;
            loader->index = i;
        }
    }
    for (i = 0; i < 1; i++) {
        if ((loader->slots[loader->bank][i].valid != 1 || loader->slots[loader->bank][i].magic != 0x73646174) && best != 0) {
            best = 0;
            loader->index = i;
        }
    }
    for (i = 0; i < 1; i++) {
        if (loader->slots[loader->bank][i].count > found && loader->slots[loader->bank][i].valid == 1 && loader->slots[loader->bank][i].magic == 0x73646174) {
            found = loader->slots[loader->bank][i].count;
        }
    }
    loader->index++;
    if (loader->index >= 1) {
        loader->index = 0;
    }
    func_0207303c(src,loader->buffer + 0x20,size);
    loader->slots[loader->bank][loader->index].count = found + 1;
    loader->slots[loader->bank][loader->index].magic = 0x73646174;
    loader->slots[loader->bank][loader->index].size = size;
    loader->slots[loader->bank][loader->index].valid = 1;
    loader->slots[loader->bank][loader->index].kind = 0xd;
    loader->slots[loader->bank][loader->index].time = func_01ff80b8(func_02008eb0());
    reader = func_02008b34(&data_020b4280);
    if (reader != 0) {
        loader->slots[loader->bank][loader->index].time = func_02008b78(reader,loader->work,loader->buffer + 0x20,size);
        if (reader != 0) {
            delete reader;
        }
    }
    func_0207303c(&loader->slots[loader->bank][loader->index],loader->buffer,0x20);
    func_02073ed4(loader->thread,func_02008c98,loader,loader->stack + sizeof(loader->stack),0x1000,5);
    func_02074188(loader->thread);
    return 1;
}
}
