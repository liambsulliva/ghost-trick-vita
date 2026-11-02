struct Context;
struct Writer;
struct Field;

struct List {
    int vtbl;
    int head;
};

struct Iter {
    struct Field *node;
};

struct Vec {
    int x;
    int y;
    int z;
};

class Object {
public:
    virtual void v00();
    virtual void v04();
    virtual void v08();
    virtual void fields(struct List *list);
};

struct Ref {
    unsigned int expand : 1;
    unsigned int type : 15;
    unsigned int index : 16;
};

union RefWord {
    struct Ref bits;
    unsigned int word;
};

extern "C" {
int func_0201ec50(struct Writer *writer, unsigned int value);
unsigned int func_02021dcc(struct Context *ctx, Object *object);
int func_020222d0(struct Context *ctx, Object *object);
int func_020222c8(struct Context *ctx, int index);
int func_020222f4(struct Context *ctx, Object *object);
void func_0201ed38(struct Writer *writer);
int func_0201eca0(struct Writer *writer, Object *object);
void func_020222c0(struct Context *ctx, int index);
void func_02020c98(struct List *list);
struct Field *func_02020cfc(struct List *list);
void func_02020cd8(struct Iter *iter, struct Field *node);
struct Field *func_02020cdc(struct Iter *iter);
struct Field *func_02020cf8(struct List *list);
struct Field *func_02020ce0(struct Iter *iter, int unused);
struct List *func_02020ca8(struct List *list);
int func_02021acc(struct Context *ctx, struct Field *field, int mode);
int func_0202038c(struct Field *field);
unsigned int func_02020ad0(struct Field *field);
void func_02020e50(struct Field *field, unsigned int index);
int func_02020254(struct Field *field);
unsigned int func_02020e54(struct Field *field);
Object *func_02020b3c(struct Field *field);
int func_02020724(struct Field *field);
unsigned char func_02020788(struct Field *field);
unsigned short func_020207ec(struct Field *field);
unsigned int func_02020854(struct Field *field);
unsigned long long func_020208bc(struct Field *field);
signed char func_02020928(struct Field *field);
short func_0202098c(struct Field *field);
int func_020209f4(struct Field *field);
long long func_02020a5c(struct Field *field);
void func_02022240(struct Vec *vec, struct Field *field);
void func_0201eaf0(struct Writer *writer, unsigned char value);
void func_020220cc(struct Writer *writer, unsigned short value);
void func_0202207c(struct Writer *writer, unsigned long long value);
void func_0202220c(struct Writer *writer, signed char value);
void func_020221bc(struct Writer *writer, short value);
void func_0202216c(struct Writer *writer, int value);
void func_0202211c(struct Writer *writer, long long value);
void func_0201eae0(struct Writer *writer, void *data, unsigned int size);
int func_0201eb3c(struct Writer *writer, Object *object);
void func_0201ed90(struct Writer *writer);

void func_02021e08(struct Context *ctx, struct Writer *writer, Object *object, int expand, int mode)
{
    union RefWord ref;
    struct List list;
    struct Iter iter;
    struct Vec vec;
    struct Field *field;
    unsigned int count;
    unsigned int i;
    int index;

    if (object == 0) {
        ref.bits.expand = 0;
        ref.bits.type = 0x7fff;
        ref.bits.index = 0;
        func_0201ec50(writer, ref.word);
        return;
    }
    ref.bits.expand = expand;
    ref.bits.type = func_02021dcc(ctx, object);
    index = func_020222d0(ctx, object);
    if (index >= 0) {
        ref.bits.index = index;
        if (func_020222c8(ctx, ref.bits.index)) {
            ref.bits.expand = 0;
        }
    } else {
        ref.bits.index = func_020222f4(ctx, object);
    }
    func_0201ec50(writer, ref.word);
    if (ref.bits.expand == 0) {
        return;
    }
    func_0201ed38(writer);
    func_0201eca0(writer, object);
    func_020222c0(ctx, ref.bits.index);
    func_02020c98(&list);
    object->fields(&list);
    for (func_02020cd8(&iter, func_02020cfc(&list)); func_02020cdc(&iter) != func_02020cf8(&list); func_02020ce0(&iter, 0)) {
        field = func_02020cdc(&iter);
        if (func_02021acc(ctx, field, mode) == 0) {
            continue;
        }
        count = 1;
        if (func_0202038c(field)) {
            count = func_02020ad0(field);
            func_0201ec50(writer, count);
        }
        for (i = 0; i < count; i++) {
            func_02020e50(field, i);
            switch (func_02020254(field)) {
            case 1:
            case 2:
                if (func_02020e54(field) & 0x10) {
                    func_02021e08(ctx, writer, func_02020b3c(field), 0, mode);
                } else {
                    func_02021e08(ctx, writer, func_02020b3c(field), 1, mode);
                }
                break;
            case 3:
                if (func_02020724(field)) {
                    func_0201eaf0(writer, 1);
                } else {
                    func_0201eaf0(writer, 0);
                }
                break;
            case 7:
                func_0202207c(writer, func_020208bc(field));
                break;
            case 6:
                func_0201ec50(writer, func_02020854(field));
                break;
            case 5:
                func_020220cc(writer, func_020207ec(field));
                break;
            case 4:
                func_0201eaf0(writer, func_02020788(field));
                break;
            case 11:
                func_0202211c(writer, func_02020a5c(field));
                break;
            case 10:
                func_0202216c(writer, func_020209f4(field));
                break;
            case 9:
                func_020221bc(writer, func_0202098c(field));
                break;
            case 8:
                func_0202220c(writer, func_02020928(field));
                break;
            case 14:
                func_02022240(&vec, field);
                func_0201eae0(writer, &vec, 12);
                break;
            }
        }
    }
    func_0201eb3c(writer, object);
    func_0201ed90(writer);
    func_02020ca8(&list);
}
}
