struct Pair {
    int first;
    int second;
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
    int offset;
    int f0c;
    struct Pair p10;
    struct Pair p18;
    struct Pair p20;
};

struct Field {
    int word0;
    unsigned short type;
    unsigned short flags;
    int value;
    int offset;
    int f10;
    struct Pair p14;
    struct Pair p1c;
    struct Pair p24;
    int f2c;
    int f30;
    int f34;
};

struct Vec {
    int x;
    int y;
    int z;
};

extern void func_02020c64(struct Field *field);
extern unsigned int func_02021200(int reader);
extern unsigned long long func_02021244(int reader);
extern unsigned short func_02021288(int reader);
extern long long func_020212d0(int reader);
extern int func_02021314(int reader);
extern short func_02021358(int reader);
extern unsigned char func_0201ea40(int reader);
extern signed char func_0201e9ec(int reader);
extern void func_0201ea1c(int reader, void *dst, unsigned int size);
extern void func_0201ea28(int reader, unsigned int size);
extern void *func_02021658(int ctx, int reader, void *object);
extern void func_02020b04(struct Field *field, unsigned int count);
extern void func_02020e50(struct Field *field, unsigned int index);
extern void *func_02020b3c(struct Field *field);
extern void func_02020e58(struct Field *field, void *object);
extern void func_02020310(struct Field *field, int value);
extern void func_020204e0(struct Field *field, unsigned long long value);
extern void func_02020474(struct Field *field, unsigned int value);
extern void func_02020408(struct Field *field, unsigned short value);
extern void func_0202039c(struct Field *field, unsigned char value);
extern void func_0202069c(struct Field *field, long long value);
extern void func_02020630(struct Field *field, int value);
extern void func_020205c4(struct Field *field, short value);
extern void func_02020558(struct Field *field, signed char value);
extern void func_0200f70c(struct Vec *vec);
extern void func_02020bec(struct Field *field, struct Vec *vec);

void func_02021070(int ctx, int reader, int value, struct Desc *desc)
{
    struct Field field;
    struct Vec vec;
    unsigned int count;
    unsigned int i;

    func_02020c64(&field);
    field.flags = desc->info.flags;
    field.word0 = desc->word0;
    field.type = desc->info.type;
    field.value = value;
    field.p14.first = desc->f0c;
    field.p1c = desc->p18;
    field.p24 = desc->p20;
    field.offset = desc->offset;
    field.f10 = desc->f0c;
    field.p14 = desc->p10;
    field.f2c = 0;
    count = 1;
    if (desc->info.flags & 0x20) {
        count = func_02021200(reader);
        func_02020b04(&field, count);
    }
    for (i = 0; i < count; i++) {
        func_02020e50(&field, i);
        switch (desc->info.type) {
        case 1:
        case 2:
            func_02020e58(&field, func_02021658(ctx, reader, func_02020b3c(&field)));
            break;
        case 3:
            func_02020310(&field, func_0201ea40(reader) != 0);
            break;
        case 7:
            func_020204e0(&field, func_02021244(reader));
            break;
        case 6:
            func_02020474(&field, func_02021200(reader));
            break;
        case 5:
            func_02020408(&field, func_02021288(reader));
            break;
        case 4:
            func_0202039c(&field, func_0201ea40(reader));
            break;
        case 11:
            func_0202069c(&field, func_020212d0(reader));
            break;
        case 10:
            func_02020630(&field, func_02021314(reader));
            break;
        case 9:
            func_020205c4(&field, func_02021358(reader));
            break;
        case 8:
            func_02020558(&field, func_0201e9ec(reader));
            break;
        case 14:
            func_0200f70c(&vec);
            func_0201ea1c(reader, &vec, 12);
            func_02020bec(&field, &vec);
            break;
        default:
            func_0201ea28(reader, desc->info.size);
            break;
        }
    }
}
