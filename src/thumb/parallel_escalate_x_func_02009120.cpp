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

struct Order {
    signed char index;
    unsigned char pad[3];
};

struct Loader {
    unsigned char head[4];
    unsigned short kind;
    unsigned char status;
    signed char index;
    unsigned char bank;
    unsigned char busy;
    unsigned char pad10[2];
    unsigned char *buffer;
    unsigned char *dest;
    unsigned int destSize;
    unsigned char marked;
    unsigned char pad19;
    unsigned char reset;
    unsigned char pad1b;
    unsigned char work[0x400];
    unsigned char thread[0xc0];
    unsigned char stack[0x1000];
    struct Order order[1];
    struct Entry slots[4][1];
};

struct Bank {
    unsigned int base;
    unsigned int size;
};

extern "C" {
extern const struct Bank data_02083a8c[];
extern char data_020b4280;
extern unsigned char func_0200902c(struct Loader *loader);
extern void func_02009750(struct Loader *loader);
extern void func_02009760(struct Loader *loader);
extern void func_02009110(void *object);
extern int func_0200935c(unsigned int src,void *dst,unsigned int size,void (*done)(void *),void *arg);
extern int func_0206a8bc(void);
extern void func_020740d0(int value);
extern int func_02069b94(void);
extern void *func_02007bd8(void);
extern Manager *func_02007b70(void *system);
extern void *func_02008eb0(void);
extern int func_01ff80b8(void *object);
extern Reader *func_02008b34(void *heap);
extern int func_02008b78(Reader *reader,void *first,void *second,unsigned int third);
extern void func_0207303c(void *src,void *dst,unsigned int size);
extern void func_020094b8(struct Loader *loader);
extern void func_02009630(struct Loader *loader,unsigned int mode,void *entry);

void func_02009120(struct Loader *loader)
{
    unsigned char status;
    int done;
    int bad;
    int retried;
    int reset;
    unsigned int i;
    int failed;
    int busy;
    unsigned char *buf;
    int match;
    unsigned char *dst;
    int time;
    Reader *reader;
    Manager *manager;

    status = func_0200902c(loader);
    done = 0;
    bad = 0;
    retried = 0;
    reset = 0;
    failed = 0;
    if (loader->reset != 0) {
        reset = 1;
        loader->reset = 0;
    }
    busy = 1;
    if (status == 1) {
        func_02009750(loader);
        for (i = 0; i < 1; i++) {
            loader->index = loader->order[i].index;
            buf = loader->buffer;
            func_0200935c(data_02083a8c[loader->bank].base + loader->index * data_02083a8c[loader->bank].size,buf,data_02083a8c[loader->bank].size,func_02009110,loader);
            while (func_0206a8bc() == 0) {
                func_020740d0(0);
            }
            busy = func_02069b94();
            if (loader->slots[loader->bank][loader->index].valid == 0 || loader->slots[loader->bank][loader->index].magic != 0x73646174) {
                bad = 1;
                continue;
            }
            match = 0;
            if (loader->slots[loader->bank][loader->index].valid == 1) {
                dst = buf + 0x20;
                time = func_01ff80b8(func_02008eb0());
                reader = func_02008b34(&data_020b4280);
                if (reader != 0) {
                    time = func_02008b78(reader,loader->work,dst,loader->slots[loader->bank][loader->index].size);
                    if (reader != 0) {
                        delete reader;
                    }
                }
                if (time == loader->slots[loader->bank][loader->index].time) {
                    match = 1;
                }
                if (loader->slots[loader->bank][loader->index].kind != 0xd) {
                    reset = 1;
                    continue;
                }
                if (match == 0) {
                    failed = 1;
                    continue;
                }
                func_0207303c(buf + 0x20,loader->dest,loader->destSize);
                if (failed != 0) {
                    retried = 1;
                }
                done = 1;
                break;
            }
            failed = 1;
        }
        func_02009760(loader);
    }
    if (loader->buffer != 0) {
        if (func_02007b70(func_02007bd8()) != 0) {
            manager = func_02007b70(func_02007bd8());
            manager->Free(loader->buffer);
            loader->buffer = 0;
        }
    }
    if (busy == 0 && done != 0 && failed == 0 && reset == 0) {
        loader->status = 1;
        return;
    }
    if (status != 1) {
        loader->status = 2;
        return;
    }
    if (busy != 0) {
        loader->status = 2;
        return;
    }
    if (reset != 0) {
        loader->status = 6;
        func_020094b8(loader);
        loader->busy = 0;
        return;
    }
    if (retried != 0) {
        loader->status = 4;
        return;
    }
    if (failed != 0) {
        if (func_0200902c(loader) != 1) {
            loader->status = 2;
            return;
        }
        loader->status = 3;
        func_020094b8(loader);
        func_02009630(loader,0,loader->slots);
        loader->busy = 0;
        return;
    }
    if (bad != 0) {
        loader->status = 5;
        return;
    }
    loader->status = 2;
}
}
