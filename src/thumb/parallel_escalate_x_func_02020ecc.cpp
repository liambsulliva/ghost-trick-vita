#pragma opt_common_subs off

class Heap {
public:
    virtual void slot0();
    virtual void slot1();
    virtual void *alloc(int size, char *name);
    virtual void slot3();
    virtual void free(void *ptr);
};

class Source {
public:
    virtual void slot0();
    virtual void slot1();
    virtual int ready();
};

struct Member {
    int ptr;
    unsigned char pad4[0x24];
};

struct Class {
    char *name;
    unsigned int count : 15;
    unsigned int rest : 17;
    struct Member members[1];
};

struct Header {
    int magic;
    unsigned short flags;
    unsigned short version;
    int count;
    int size;
};

struct Reader;

struct Loader {
    int state;
    void *work;
    void *names;
    struct Class **table;
    unsigned int count;
};

extern "C" {
extern char data_02084f14[];
extern char data_020bbbb4[];
extern void func_0201e9d0(struct Reader *rd, Source *src, int size);
extern void func_0201e9dc(struct Reader *rd);
extern void func_0201ea1c(struct Reader *rd, void *dst, int size);
extern void *func_02007bd8(void);
extern Heap *func_02007b70(void *object);
extern char *func_0201ee6c(char *name, char *table);
extern void func_02072fa8(void *dst, int value, int size);
extern void *func_02021658(struct Loader *ctx, struct Reader *rd, void *root);

struct Reader {
    int a;
    int b;
};

void *func_02020ecc(struct Loader *ctx, Source *src, unsigned short version, void *root);
}

void *func_02020ecc(struct Loader *ctx, Source *src, unsigned short version, void *root)
{
    struct Header hdr;
    struct Reader rd;
    char *data;
    unsigned int i;
    unsigned int n;
    unsigned int j;
    unsigned int off;
    struct Class *klass;
    void *result;

    if (src->ready() == 0) {
fail:
        return 0;
    }
    func_0201e9d0(&rd, src, 0x1000);
    func_0201ea1c(&rd, &hdr, 0x10);
    if (hdr.magic != 0x534658) {
        func_0201e9dc(&rd);
        goto fail;
    }
    if (hdr.flags != 0) {
        func_0201e9dc(&rd);
        goto fail;
    }
    if (hdr.version != version) {
        func_0201e9dc(&rd);
        goto fail;
    }
    data = (char *)func_02007b70(func_02007bd8())->alloc(hdr.size, data_02084f14);
    func_0201ea1c(&rd, data, hdr.size);
    ctx->table = (struct Class **)data;
    ctx->count = hdr.count;
    for (i = 0; i < ctx->count; i++) {
        off = i * 4;
        *(char **)((char *)ctx->table + off) = data + *(int *)((char *)ctx->table + off);
        klass = *(struct Class **)((char *)ctx->table + off);
        klass->name = func_0201ee6c(klass->name, data_020bbbb4);
        n = klass->count;
        for (j = 0; j < n; j++) {
            klass->members[j].ptr = (int)(data + klass->members[j].ptr);
        }
    }
    ctx->state = 0;
    ctx->work = func_02007b70(func_02007bd8())->alloc(0x4000, data_02084f14);
    ctx->names = func_02007b70(func_02007bd8())->alloc(0x1000, data_02084f14);
    func_02072fa8(ctx->work, 0, 0x4000);
    func_02072fa8(ctx->names, 0, 0x1000);
    result = func_02021658(ctx, &rd, root);
    if (ctx->work != 0 && func_02007b70(func_02007bd8()) != 0) {
        func_02007b70(func_02007bd8())->free(ctx->work);
        ctx->work = 0;
    }
    if (ctx->names != 0 && func_02007b70(func_02007bd8()) != 0) {
        func_02007b70(func_02007bd8())->free(ctx->names);
        ctx->names = 0;
    }
    if (data != 0 && func_02007b70(func_02007bd8()) != 0) {
        func_02007b70(func_02007bd8())->free(data);
    }
    ctx->table = 0;
    ctx->count = 0;
    func_0201e9dc(&rd);
    return result;
}

