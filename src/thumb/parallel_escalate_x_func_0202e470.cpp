struct Value;

extern "C" void func_0202ec8c(Value *v);

struct ObjFlags {
    unsigned char b0 : 1;
    unsigned char b1 : 1;
    unsigned char b2 : 1;
    unsigned char b3 : 1;
    unsigned char b4 : 1;
    unsigned char b5 : 1;
    unsigned char b6 : 1;
    unsigned char b7 : 1;
};

struct List {
    unsigned char pad0[0x190];
    int list[3][7];
    unsigned char cnt[3];
};

class Obj {
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
    virtual void slot13(int id);
    virtual void slot14();
    virtual void slot15();
    virtual void slot16();
    virtual void slot17();
    virtual void slot18();
    virtual void slot19();
    virtual void slot20(int value);
    virtual Obj *slot21(short id);
    virtual void slot22(int sel, int argc, Value *args, int *depth, Value *stack);
    virtual void slot23(short value);
    virtual void slot24();
    virtual void slot25(short value);
    virtual void slot26();
    virtual void slot27(short value);
    virtual void slot28();
    virtual void slot29();
    virtual void slot30(int value);
    virtual void slot31(int value);

    unsigned char pad4[2];
    unsigned char b06;
    unsigned char pad7;
    unsigned short h08;
    unsigned short h0a;
    unsigned short h0c;
    unsigned short h0e;
    unsigned short h10;
    unsigned char pad12[0x5e];
    int w70;
    unsigned char pad74[0xc];
    List *p80;
    unsigned char pad84[2];
    ObjFlags flags;
    unsigned char pad87;
    int w88;
    unsigned char pad8c[4];
    unsigned char b90[4];
    int w94[4];
    int wa4[4];
    unsigned char padb4[0x28];
    int wdc;
};

struct Value {
    unsigned char kind;
    unsigned char type;
    unsigned char pad2[2];
    int value;
    Obj *ptr;
    unsigned char isref;
    unsigned char padd[3];
    Value() { func_0202ec8c(this); }
    Value(const Value &src);
};

struct Arg {
    unsigned char kind;
    unsigned char type;
    unsigned char sub;
    unsigned char pad3;
    int value;
};

extern "C" {
extern char data_020ca280;

void func_0202ec8c(Value *v);
void func_0202ec94(Value *dst, Value *src);
int func_0202eca8(Value *v);
void func_0202ecf8(Value *v, int value);
void func_0202ef70(Value *dst, void *src);
Value func_0202ee7c(Value *src);
void func_0202ed3c(Value *v);
void func_0202ed84(Value *dst, Obj *o, Value *src);
void func_0202eda4(Value *a, Value *b);
Value func_0202ee90(Value *a, Value *b, int op);
int func_0202ed80(Obj *o);
Obj *func_0202b230(void *tag);
void func_0202b4fc(Obj *child, int owner);
void *func_02008eb0(void);
unsigned int func_01ff80b8(void *random);

void func_0202e470(Obj *obj, unsigned char *code, int arg, Value *frame, int *count, void *ret)
{
    int depth;
    int x6;
    int b4;
    int a4;
    int cond;
    Value *out;
    int step;
    Obj *o;
    Obj *n;
    int argc;
    int top;
    int i;
    int j;
    int i1;
    int j1;
    Obj *target;
    int a;
    int b;
    int r;
    int k;
    unsigned short m;

    Arg *args;
    Value *va;
    Value *vb;
    Value result;
    cond = 0;
    Value stack[16];
    depth = 0;
    out = (Value *)ret;
    while (*code != 0) {
        step = code[2] * 8 + 4;
        switch (*code) {
        case 0x3e:
            if (depth > 0 && out != 0) {
                depth--;
                func_0202ec94(&out[*count], &stack[depth]);
                (*count)++;
            }
            return;
        case 7:
            obj->flags.b0 = 1;
            break;
        case 6:
            obj->flags.b0 = 0;
            break;
        case 0x30:
            r = func_0202eca8(&stack[depth - 1]);
            if (r == 0) {
                func_0202ecf8(&stack[depth - 1], 0);
            } else {
                func_0202ecf8(&stack[depth - 1], func_01ff80b8(func_02008eb0()) % r);
            }
            break;
        case 0x81:
            obj->w88 = ((Arg *)(code + 4))->value;
            break;
        case 0x8c:
            obj->slot23(((Arg *)(code + 4))->value);
            break;
        case 0x96:
            args = (Arg *)(code + 4);
            for (m = 0; m < code[2]; m++) {
                func_0202ef70(&stack[depth], args + m);
                stack[depth].ptr = 0;
                stack[depth].isref = 0;
                depth++;
            }
            break;
        case 0x17:
            if (depth > 0) {
                depth--;
            }
            break;
        case 0x1c:
        case 0x22:
        case 0x4e:
            depth--;
            func_0202ef70(&result, &stack[depth]);
            result.isref = stack[depth].isref;
            func_0202ed3c(&result);
            if (*code == 0x1c) {
                result.ptr = obj;
            } else {
                depth--;
                result.ptr = stack[depth].ptr;
            }
            switch (result.type) {
            case 1:
                result.ptr = obj;
                break;
            case 2:
                result.ptr = (Obj *)obj->p80;
                break;
            case 3:
                result.ptr = (Obj *)obj->w70;
                break;
            case 4:
                result.ptr = result.ptr->slot21(result.value);
                break;
            case 7:
                func_0202ec94(&stack[depth++], &frame[-result.value]);
                break;
            }
            if (result.type != 7) {
                func_0202ec94(&stack[depth++], &result);
            }
            break;
        case 0x3d:
        case 0x52:
            depth--;
            func_0202ec94(&result, &stack[depth]);
            o = obj;
            if (*code == 0x52) {
                depth--;
                o = stack[depth].ptr;
            }
            depth--;
            argc = stack[depth].value;
            switch (result.value) {
            case 0x8001:
                if (stack[depth - 1].kind & 1) {
                    if (o != 0) {
                        o->slot31(stack[depth - 1].value);
                    }
                } else {
                    if (o != 0) {
                        o->slot27(stack[depth - 1].value);
                    }
                }
                depth -= argc;
                break;
            case 0x8000:
                if (stack[depth - 1].kind & 1) {
                    if (o != 0) {
                        o->slot30(stack[depth - 1].value);
                    }
                } else {
                    if (o != 0) {
                        o->slot25(stack[depth - 1].value);
                    }
                }
                depth -= argc;
                break;
            case 0x8003:
                if (depth > 0) {
                    a = func_0202eca8(&stack[depth - 1]);
                    if (depth > 1) {
                        b = func_0202eca8(&stack[depth - 2]);
                        if (a >= 0 && a <= 2) {
                            obj->p80->list[a][obj->p80->cnt[a]] = b;
                            obj->p80->cnt[a]++;
                        }
                    }
                }
                depth -= argc;
                break;
            case 0x8004:
                if (argc == 2) {
                    a4 = func_0202eca8(&stack[depth - 1]);
                    b4 = func_0202eca8(&stack[depth - 2]);
                    n = func_0202b230(&data_020ca280);
                    n->slot13(func_0202ed80(o));
                    func_0202b4fc(n, o->w70);
                    n->w88 = 0;
                    n->h0a = b4;
                    n->h0c = o->h0c;
                    n->h0e = 0xffff;
                    n->h10 = o->h10;
                    n->h08 = a4;
                    n->b06 = 0;
                    n->slot20(o->wdc);
                }
                depth -= argc;
                break;
            case 0x8005:
                if (o != 0) {
                    o->flags.b3 = 1;
                } else {
                    obj->flags.b3 = 1;
                }
                depth -= argc;
                break;
            case 0x8006:
                if (argc == 3) {
                    i = func_0202eca8(&stack[depth - 1]);
                    x6 = func_0202eca8(&stack[depth - 2]);
                    j = func_0202eca8(&stack[depth - 3]);
                    if (i >= 0 && i < 4) {
                        o->b90[i] = 1;
                        o->w94[i] = x6;
                        o->wa4[i] = j;
                    }
                }
                depth -= argc;
                break;
            case 0x8007:
                if (o != 0) {
                    o->flags.b0 = 0;
                }
                depth -= argc;
                break;
            default:
                top = depth;
                depth -= argc;
                if (o != 0) {
                    o->slot22(result.value, argc, &stack[top - 1], &depth, stack);
                }
                break;
            }
            break;
        case 0x26:
            depth--;
            break;
        case 0x1d:
        case 0x23:
        case 0x4f:
            i1 = --depth;
            j1 = --depth;
            target = obj;
            if (*code != 0x1d) {
                depth--;
                target = stack[depth].ptr;
            }
            func_0202ed3c(&stack[j1]);
            func_0202ed84(&stack[j1], target, &stack[i1]);
            break;
        case 0x83:
            if (((Arg *)(code + 4))->type == 0xd) {
                a = ((Arg *)(code + 4))->sub;
                b = ((Arg *)(code + 4))->value;
                if ((unsigned int)a <= 2) {
                    obj->p80->list[a][obj->p80->cnt[a]] = b;
                    obj->p80->cnt[a]++;
                }
            }
            break;
        case 0x9a:
            i = --depth;
            j = --depth;
            if (stack[j].type != 5) {
                a = func_0202eca8(&stack[j]);
                b = func_0202eca8(&stack[i]);
                if (a >= 0 && a <= 2) {
                    obj->p80->list[a][obj->p80->cnt[a]] = b;
                    obj->p80->cnt[a]++;
                }
            }
            break;
        case 0x21:
            depth--;
            break;
        case 0xe:
        case 0x49:
            va = &stack[--depth];
            vb = &stack[--depth];
            func_0202eda4(va, vb);
            result.kind = 1;
            result.value = 0;
            if (func_0202eca8(va) == func_0202eca8(vb)) {
                result.value = 1;
            }
            func_0202ec94(&stack[depth++], &result);
            break;
        case 0xf:
        case 0x48:
            va = &stack[--depth];
            vb = &stack[--depth];
            func_0202eda4(va, vb);
            result.kind = 1;
            result.value = 0;
            if (func_0202eca8(va) > func_0202eca8(vb)) {
                result.value = 1;
            }
            func_0202ec94(&stack[depth++], &result);
            break;
        case 0x67:
            va = &stack[--depth];
            vb = &stack[--depth];
            func_0202eda4(va, vb);
            result.kind = 1;
            result.value = 0;
            if (func_0202eca8(va) < func_0202eca8(vb)) {
                result.value = 1;
            }
            func_0202ec94(&stack[depth++], &result);
            break;
        case 0x12:
            stack[depth - 1].value = 1 - stack[depth - 1].value;
            break;
        case 0x10:
        case 0x11:
            depth--;
            a = func_0202eca8(&stack[depth]);
            depth--;
            b = func_0202eca8(&stack[depth]);
            result.kind = 1;
            if (*code == 0x10) {
                result.value = a & b;
            } else {
                result.value = a | b;
            }
            func_0202ec94(&stack[depth++], &result);
            break;
        case 0x4d:
            {
                Value t1 = func_0202ee7c(&stack[depth - 1]);
                func_0202ec94(&stack[depth - 1], &stack[depth - 2]);
                func_0202ec94(&stack[depth - 2], &t1);
            }
            break;
        case 0x4c:
            func_0202ec94(&stack[depth], &stack[depth - 1]);
            stack[depth].isref = 0;
            stack[depth].value = func_0202eca8(&stack[depth - 1]);
            depth++;
            break;
        case 0x99:
            cond = 1;
        case 0x9d:
            if (*code == 0x9d) {
                depth--;
                cond = stack[depth].value;
            }
            if (cond != 0) {
                args = (Arg *)(code + 4);
                code += step;
                if (args->value > 0) {
                    for (k = 0; k < args->value; k++) {
                        code += code[2] * 8 + 4;
                    }
                } else {
                    for (k = 0; k < -args->value; k++) {
                        code -= code[3] * 8 + 4;
                    }
                }
                step = 0;
            }
            break;
        case 0xa:
        case 0xb:
        case 0xc:
        case 0xd:
        case 0x3f:
        case 0x47:
            va = &stack[--depth];
            vb = &stack[--depth];
            {
                Value t2 = func_0202ee90(va, vb, *code);
                func_0202ec94(&stack[depth++], &t2);
            }
            break;
        case 0x50:
        case 0x51:
            r = func_0202eca8(&stack[depth - 1]) - 1;
            if (*code == 0x50) {
                r += 2;
            }
            func_0202ecf8(&stack[depth - 1], r);
            break;
        }
        code += step;
    }
}
}
