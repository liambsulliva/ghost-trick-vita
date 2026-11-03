struct Desc {
    char *name;
    unsigned int info;
    int pad8[8];
};

struct ClassBits {
    unsigned int count : 15;
    unsigned int rest : 17;
};

union ClassWord {
    struct ClassBits bits;
    unsigned int word;
};

struct Class {
    int key;
    union ClassWord u;
    struct Desc fields[1];
};

class Object;

struct Context {
    int count;
    Object **objects;
    unsigned char *visited;
    struct Class **classes;
    unsigned int class_count;
    char *pool;
    int pool_size;
};

struct Writer {
    char data[0x614];
};

struct Header {
    int magic;
    unsigned short f4;
    unsigned short version;
    unsigned int count;
    unsigned int size;
};

class Stream {
public:
    virtual void v00();
    virtual void v04();
    virtual void v08();
    virtual void v0c();
    virtual int ready();
};

class Heap {
public:
    virtual void v00();
    virtual void v04();
    virtual void *alloc(int size, char *tag);
    virtual void v0c();
    virtual void free(void *block);
};

extern "C" {
extern char data_02084f14;
void *func_02007bd8(void);
Heap *func_02007b70(void *system);
void func_02072fa8(void *dst, int value, unsigned int size);
void func_02021b44(struct Context *ctx, Object *object, int mode);
void func_0201ea74(struct Writer *writer, Stream *stream, int size, int step);
void func_0201eae0(struct Writer *writer, void *data, unsigned int size);
void func_0201ed38(struct Writer *writer);
int func_0201eca0(struct Writer *writer, void *object);
int func_0201eb3c(struct Writer *writer, void *object);
int func_0201ec50(struct Writer *writer, unsigned int value);
void func_0201ed90(struct Writer *writer);
void func_02021e08(struct Context *ctx, struct Writer *writer, Object *object, int expand, int mode);
void func_0201eac0(struct Writer *writer);
int func_02021788(struct Context *ctx, Stream *stream, int version, Object *root, int mode);
}

int func_02021788(struct Context *ctx, Stream *stream, int version, Object *root, int mode)
{
    struct Writer writer;
    struct Header header;
    struct Desc *desc;
    unsigned int base;
    struct Class *cls;
    unsigned int i;
    unsigned int j;
    unsigned int k;

    if (stream->ready() == 0) {
        return 0;
    }
    desc = (struct Desc *)func_02007b70(func_02007bd8())->alloc(0x4000, &data_02084f14);
    ctx->classes = (struct Class **)desc;
    func_02072fa8(desc, 0, 0x4000);
    ctx->class_count = 0;
    ctx->pool = (char *)func_02007b70(func_02007bd8())->alloc(0x8000, &data_02084f14);
    func_02072fa8(ctx->pool, 0, 0x8000);
    ctx->pool_size = 0;
    ctx->count = 0;
    ctx->objects = (Object **)func_02007b70(func_02007bd8())->alloc(0x4000, &data_02084f14);
    ctx->visited = (unsigned char *)func_02007b70(func_02007bd8())->alloc(0x1000, &data_02084f14);
    func_02021b44(ctx, root, mode);
    func_0201ea74(&writer, stream, 0x1000, 0x1000);
    header.magic = 0x534658;
    header.f4 = 0;
    header.version = version;
    header.count = ctx->class_count;
    header.size = ctx->class_count * 4;
    for (i = 0; i < ctx->class_count; i++) {
        header.size += (ctx->classes[i]->u.bits.count - 1) * 0x28 + 0x30;
    }
    base = header.size;
    header.size = base + ((ctx->pool_size + 3) & ~3);
    func_0201eae0(&writer, &header, 0x10);
    func_0201ed38(&writer);
    for (i = 0; i < ctx->class_count; i++) {
        func_0201eca0(&writer, ctx->classes[i]);
    }
    for (j = 0; j < ctx->class_count; j++) {
        cls = ctx->classes[j];
        func_0201eb3c(&writer, cls);
        func_0201ec50(&writer, cls->key);
        func_0201ec50(&writer, cls->u.word);
        for (k = 0; k < cls->u.bits.count; k++) {
            desc = &cls->fields[k];
            func_0201ec50(&writer, base + (desc->name - ctx->pool));
            func_0201ec50(&writer, desc->info);
            func_0201ec50(&writer, 0);
            func_0201ec50(&writer, 0);
            func_0201ec50(&writer, 0);
            func_0201ec50(&writer, 0);
            func_0201ec50(&writer, 0);
            func_0201ec50(&writer, 0);
            func_0201ec50(&writer, 0);
            func_0201ec50(&writer, 0);
        }
    }
    func_0201eae0(&writer, ctx->pool, (ctx->pool_size + 3) & ~3);
    func_0201ed90(&writer);
    ctx->count = 0;
    func_02021e08(ctx, &writer, root, 1, mode);
    if (ctx->objects != 0 && func_02007b70(func_02007bd8()) != 0) {
        func_02007b70(func_02007bd8())->free(ctx->objects);
        ctx->objects = 0;
    }
    if (ctx->visited != 0 && func_02007b70(func_02007bd8()) != 0) {
        func_02007b70(func_02007bd8())->free(ctx->visited);
        ctx->visited = 0;
    }
    for (i = 0; i < ctx->class_count; i++) {
        if (ctx->classes[i] != 0 && func_02007b70(func_02007bd8()) != 0) {
            func_02007b70(func_02007bd8())->free(ctx->classes[i]);
            ctx->classes[i] = 0;
        }
    }
    if (ctx->classes != 0 && func_02007b70(func_02007bd8()) != 0) {
        func_02007b70(func_02007bd8())->free(ctx->classes);
        ctx->classes = 0;
    }
    if (ctx->pool != 0 && func_02007b70(func_02007bd8()) != 0) {
        func_02007b70(func_02007bd8())->free(ctx->pool);
        ctx->pool = 0;
    }
    ctx->classes = 0;
    ctx->class_count = 0;
    ctx->pool = 0;
    ctx->pool_size = 0;
    func_0201eac0(&writer);
    return 1;
}
