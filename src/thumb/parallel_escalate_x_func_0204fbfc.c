typedef struct Obj0204fbfc {
    char pad0[0x28];
    unsigned char b28;
    unsigned char b29;
    char pad2a[0xe];
    unsigned int w38;
    unsigned int w3c;
    unsigned int w40;
    char pad44[4];
    void *list[5];
} Obj0204fbfc;

typedef struct Ids0204fbfc {
    unsigned char v[2];
} Ids0204fbfc;

extern const Ids0204fbfc data_0209be5c;
extern int func_02008444();
extern int func_020322d4();
extern int func_02013a14();
extern int func_0204fdb0();
extern int func_0201132c();
extern unsigned char func_02010fa8(Obj0204fbfc *obj, signed char idx);
extern void func_0204f290(void *part, int a, int b);
extern int func_0204faf4(void *part);
extern void func_0204fafc(void *part, int state);
extern void func_0204b120(void *part, int frame);

void func_0204fbfc(Obj0204fbfc *obj, int idx, int mode, unsigned char a, unsigned char b)
{
    unsigned char *flags[2];
    Ids0204fbfc ids;
    unsigned char vals[2];
    int i;

    if (mode == 2) {
        obj->w38 &= ~(1 << idx);
        obj->w3c &= ~(1 << idx);
    }
    if (mode == 3) {
        obj->w38 |= 1 << idx;
        obj->w40 &= ~(1 << idx);
    }
    if (mode == 5) {
        obj->w38 |= 1 << idx;
        obj->w40 &= ~(1 << idx);
        return;
    }
    if (mode == 6) {
        obj->w38 &= ~(1 << idx);
        obj->w3c &= ~(1 << idx);
        return;
    }
    ids = data_0209be5c;
    vals[0] = a;
    vals[1] = b;
    flags[0] = &obj->b28;
    flags[1] = &obj->b29;
    for (i = 0; i < 2; i++) {
        if (idx == ids.v[i]) {
            if (vals[i] != 0) {
                if (mode & 5) {
                    *flags[i] = 0;
                } else {
                    *flags[i] = 1;
                }
            } else {
                *flags[i] = 0;
            }
        }
    }
    if (idx == 1) {
        if (func_020322d4(func_02008444()) == 0) {
            return;
        }
        if (mode == 1 || mode == 3) {
            if (func_02013a14(func_02008444()) == 0) {
                return;
            }
            if (func_0204fdb0(func_02008444()) == 0) {
                return;
            }
        }
    }
    if (mode == 4) {
        if (func_02010fa8(obj, idx) == 0) {
            return;
        }
        func_0204f290(obj->list[idx], 4, -1);
        return;
    }
    if (mode & 1) {
        if (func_02010fa8(obj, idx) == 0) {
            return;
        }
        if (func_0204faf4(obj->list[idx]) == 1) {
            return;
        }
        if (func_0204faf4(obj->list[idx]) == 4) {
            return;
        }
        if (func_0204faf4(obj->list[idx]) == 2) {
            return;
        }
        func_0204f290(obj->list[idx], 2, -1);
        if (func_0201132c(func_02008444()) != 0) {
            func_0204b120(obj->list[idx], 1);
        } else {
            func_0204b120(obj->list[idx], 0);
        }
        return;
    }
    if (func_0204faf4(obj->list[idx]) == 0) {
        return;
    }
    if (mode == 2) {
        func_0204f290(obj->list[idx], 0, -1);
        return;
    }
    if (idx == 4 && func_0204faf4(obj->list[idx]) == 4) {
        func_0204fafc(obj->list[idx], 3);
        return;
    }
    func_0204f290(obj->list[idx], 3, -1);
}
