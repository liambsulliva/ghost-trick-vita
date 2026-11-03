union Pair {
    struct {
        int first;
        int second;
    } v;
    long long all;
};

struct Info {
    unsigned int type : 8;
    unsigned int flags : 8;
    unsigned int size : 15;
    unsigned int top : 1;
};

struct Desc {
    int word0;
    struct Info info;
    union Pair range;
    union Pair p10;
    union Pair p18;
    union Pair p20;
};

struct ClassBits {
    unsigned int count : 15;
    unsigned int rest : 17;
};

struct Class {
    int key;
    struct ClassBits bits;
    struct Desc fields[1];
};

struct Field {
    int word0;
    unsigned short type;
    unsigned short flags;
    int value;
    union Pair range;
    union Pair p14;
    union Pair p1c;
    union Pair p24;
};

struct List {
    int vtbl;
    int head;
};

struct Iter {
    struct Field *node;
};

class Object {
public:
    virtual void v00();
    virtual void v04();
    virtual void v08();
    virtual void fields(struct List *list);
    virtual void *v10();
};

struct Context {
    int count;
    Object **objects;
    unsigned char *visited;
    struct Class **classes;
    unsigned int class_count;
};

struct Manager {
    void *(**table)(struct Manager *manager, int size, char *name);
};

extern "C" {
extern char data_02084f14;
int func_0201ee8c(void *type);
int func_020222d0(struct Context *ctx, Object *object);
int func_020222f4(struct Context *ctx, Object *object);
void func_02020c98(struct List *list);
struct Field *func_02020cfc(struct List *list);
void func_02020cd8(struct Iter *iter, struct Field *node);
struct Field *func_02020cdc(struct Iter *iter);
struct Field *func_02020cf8(struct List *list);
struct Field *func_02020ce0(struct Iter *iter, int unused);
struct List *func_02020ca8(struct List *list);
int func_02021acc(struct Context *ctx, struct Field *field, int mode);
void *func_02007bd8(void);
struct Manager *func_02007b70(void *system);
void func_02072fa8(void *dst, int value, unsigned int size);
int func_02020dac(struct Field *field);
int func_02021a80(struct Context *ctx, int name);
int func_02020254(struct Field *field);
unsigned int func_02020e54(struct Field *field);
int func_020201f0(struct Field *field);
int func_0202037c(struct Field *field);
int func_02021dc8(struct Field *field);
int func_0202038c(struct Field *field);
unsigned int func_02020ad0(struct Field *field);
void func_02020e50(struct Field *field, unsigned int index);
Object *func_02020b3c(struct Field *field);
void func_02021b44(struct Context *ctx, Object *object, int mode);
}

void func_02021b44(struct Context *ctx, Object *object, int mode)
{
    struct List list;
    struct Iter iter;
    struct Iter iter2;
    struct Iter iter3;
    struct Manager *manager;
    struct Class *cls;
    struct Field *field;
    struct Desc *desc;
    Object *child;
    int key;
    int j;
    int found;
    unsigned int count;
    unsigned int i;
    int n;
    int size;
    unsigned int k;

    if (object == 0) {
        return;
    }
    key = func_0201ee8c(object->v10());
    count = ctx->class_count;
    found = 0;
    for (i = 0; i < count; i++) {
        if (key == ctx->classes[i]->key) {
            found = 1;
            break;
        }
    }
    if (func_020222d0(ctx, object) >= 0) {
        return;
    }
    func_020222f4(ctx, object);
    func_02020c98(&list);
    object->fields(&list);
    if (found == 0) {
        n = 0;
        for (func_02020cd8(&iter, func_02020cfc(&list)); func_02020cdc(&iter) != func_02020cf8(&list); func_02020ce0(&iter, 0)) {
            if (func_02021acc(ctx, func_02020cdc(&iter), mode)) {
                n++;
            }
        }
        size = (n - 1) * 0x28 + 0x30;
        manager = func_02007b70(func_02007bd8());
        cls = (struct Class *)manager->table[2](manager, size, &data_02084f14);
        j = 0;
        func_02072fa8(cls, 0, size);
        cls->bits.count = n;
        cls->key = key;
        for (func_02020cd8(&iter2, func_02020cfc(&list)); func_02020cdc(&iter2) != func_02020cf8(&list); func_02020ce0(&iter2, 0)) {
            field = func_02020cdc(&iter2);
            if (func_02021acc(ctx, field, mode)) {
                desc = &cls->fields[j];
                desc->word0 = func_02021a80(ctx, func_02020dac(field));
                desc->info.type = func_02020254(field);
                desc->info.flags = func_02020e54(field);
                desc->info.size = func_020201f0(field);
                if (func_0202037c(field)) {
                    desc->p18 = field->p1c;
                    desc->p20 = field->p24;
                    desc->range = field->range;
                    desc->p10 = field->p14;
                } else {
                    desc->range.v.first = field->range.v.first - func_02021dc8(field);
                    desc->range.v.second = field->p14.v.first;
                }
                j++;
            }
        }
        ctx->classes[ctx->class_count++] = cls;
    }
    for (func_02020cd8(&iter3, func_02020cfc(&list)); func_02020cdc(&iter3) != func_02020cf8(&list); func_02020ce0(&iter3, 0)) {
        field = func_02020cdc(&iter3);
        if (func_02021acc(ctx, field, mode)) {
            if (func_02020254(field) == 1 || func_02020254(field) == 2) {
                count = 1;
                if (func_0202038c(field)) {
                    count = func_02020ad0(field);
                }
                for (k = 0; k < count; k++) {
                    func_02020e50(field, k);
                    child = func_02020b3c(field);
                    if (child != 0) {
                        func_02021b44(ctx, child, mode);
                    }
                }
            }
        }
    }
    func_02020ca8(&list);
}
