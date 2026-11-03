typedef struct {
    int x;
    int y;
    int z;
} Vec;

class Actor {
public:
    virtual void slot0();
    virtual void slot1();
    virtual void slot2();
    virtual void slot3();
    virtual void slot4();
    virtual void slot5();
    virtual void slot6();
    virtual void slot7();
    virtual void slot8();
    virtual void slot9();
    virtual void slot10();
    virtual void slot11();
    virtual void slot12();
    virtual void slot13();
    virtual void slot14();
    virtual void slot15();
    virtual void slot16();
    virtual void slot17();
    virtual void slot18();
    virtual void slot19();
    virtual void slot20();
    virtual void slot21();
    virtual void slot22();
    virtual void slot23();
    virtual void slot24();
    virtual void slot25();
    virtual void slot26();
    virtual void slot27();
    virtual void slot28();
    virtual void slot29();
    virtual void slot30();
    virtual int slot31();
};

struct Script {
    unsigned char pad0[0x18];
    int value;
};

extern "C" {
extern unsigned char data_020cff60[];
extern char data_020cf8b4[];
extern Actor *func_02028148(struct Script *ctx, int id);
extern void *func_02008444(void);
extern int func_02010194(void *game);
extern int func_020283c4(int object);
extern void func_02058a80(Vec *out, int object);
extern int func_0200f178(void *game);
extern int func_0200f190(void *game);
extern int func_020283b0(void *game);
extern int func_02010278(void *game);
extern int func_020283b8(void *game);
extern int func_0201fa54(void *game);
extern int func_0201ff10(void *game, int index);
extern int func_0201fc30(int object);
extern int func_02011320(void *game);
extern int func_02008828(void *object, char *kind);
extern void func_0205958c(Vec *v, Actor *actor, int index);
extern int func_0200fb4c(Actor *actor);
extern int func_020283d0(Actor *actor);
extern int func_020283f0(Actor *actor);
extern int func_020283fc(Actor *actor);
extern int func_02028400(Actor *actor);
extern int func_02028404(Actor *actor);
extern int func_02028408(Actor *actor);
extern int func_0202840c(Actor *actor);
int func_020281f0(struct Script *ctx, int id);
}

int func_020281f0(struct Script *ctx, int id)
{
    Vec pos;
    Vec a;
    Vec b;
    Vec c;
    Vec d;
    Actor *actor;
    int result;
    int object;

    actor = func_02028148(ctx, id);
    result = 0;
    switch (id) {
    case 0x16:
        return func_020283b0(func_02008444());
    case 0x18:
        return func_020283b8(func_02008444());
    case 0x1e:
        return ctx->value;
    case 0x19:
        return func_0201fa54(func_02008444());
    case 0x1b:
        return func_02011320(func_02008444());
    case 0x17:
        return func_02010278(func_02008444());
    case 0x14:
        return func_0200f178(func_02008444());
    case 0x15:
        return func_0200f190(func_02008444());
    case 0x1a:
        object = func_0201ff10(func_02008444(), result);
        if (object != 0) {
            return func_0201fc30(object);
        }
        return result - 1;
    case 0x11:
        object = func_02010194(func_02008444());
        if (object != 0) {
            return func_020283c4(object);
        }
        break;
    case 0x12:
    case 0x13:
        object = func_02010194(func_02008444());
        if (object != 0) {
            func_02058a80(&pos, object);
            if (id == 0x12) {
                return pos.x;
            }
            return pos.y;
        }
        break;
    case 0x1c:
        return 1;
    case 0x1d:
        return data_020cff60[0];
    }

    if (actor != 0 && func_02008828(actor, data_020cf8b4) != 0) {
        switch (id) {
        case 0:
            func_0205958c(&a, actor, 0);
            result = a.x + func_0200fb4c(actor);
            break;
        case 1:
            func_0205958c(&b, actor, 0);
            result = b.y + func_020283d0(actor);
            break;
        case 2:
            result = actor->slot31();
            break;
        case 3:
            result = func_020283f0(actor);
            break;
        case 4:
        case 5:
        case 6:
        case 7:
            func_0205958c(&c, actor, id - 4);
            result = c.x + func_0200fb4c(actor);
            break;
        case 8:
        case 9:
        case 10:
        case 11:
            func_0205958c(&d, actor, id - 8);
            result = d.y + func_020283d0(actor);
            break;
        case 12:
            result = func_020283fc(actor);
            break;
        case 13:
            result = func_02028400(actor);
            break;
        case 14:
            result = func_02028404(actor);
            break;
        case 15:
            result = func_02028408(actor);
            break;
        case 16:
            result = func_0202840c(actor);
            break;
        default:
            result = 0;
            break;
        }
    }
    return result;
}
