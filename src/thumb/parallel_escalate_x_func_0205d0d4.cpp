class Obj0205d0d4 {
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
    virtual void slot13(int x, int y, int z);
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
    virtual void slot31();
    virtual void slot32();
    virtual void slot33();
    virtual void slot34();
    virtual void slot35(signed char value);
    virtual int slot36();
};

struct Entry0205d0d4 {
    int obj;
    int rest[4];
};

struct Vec0205d0d4 {
    int x;
    int y;
    int z;
};

extern "C" {
extern unsigned int *func_02008444(void);
extern int func_0205d398(void *object);
extern unsigned char *func_0202970c(void);
extern int func_02029708(void *object);
extern int func_02029640(void *object);
extern int func_02010b54(int object);
extern int func_0205c9a0(int object);
extern int func_02036f3c(int object);
extern void func_02058a80(struct Vec0205d0d4 *dst, int obj);
extern void func_020083b0(struct Vec0205d0d4 *dst, void *src);
extern int func_0200f5c8(void *camera);
extern unsigned int func_0200f5e8(void *camera);
extern int func_02058b4c(int obj);
extern int func_02058b64(int obj);
extern void func_02056b18(int object, unsigned short value);

void func_0205d0d4(unsigned char *p)
{
    int obj;
    unsigned int i;
    int r;
    short v;
    struct Vec0205d0d4 pos;
    int e;

    if (func_0205d398(func_02008444()) == 0) {
        func_02029708(func_0202970c());
        obj = func_02029640(p);
        if (obj == 0 || (func_02010b54(obj) != 0 && func_0205c9a0(obj) != 0)) {
            for (i = 0; i < *(unsigned int *)(p + 0x5c); i++) {
                if (func_02036f3c((*(struct Entry0205d0d4 **)(p + 0x6c))[i].obj) != 0 &&
                    (e = (*(struct Entry0205d0d4 **)(p + 0x6c))[i].obj, e != *(int *)(p + 0x68))) {
                    continue;
                }
                func_02058a80(&pos, (*(struct Entry0205d0d4 **)(p + 0x6c))[i].obj);
                e = (*(struct Entry0205d0d4 **)(p + 0x6c))[i].obj;
                if (e == *(int *)(p + 0x94)) {
                    func_020083b0(&pos, p + 0xd4);
                }
                e = (*(struct Entry0205d0d4 **)(p + 0x6c))[i].obj;
                if (e == *(int *)(p + 0x98)) {
                    func_020083b0(&pos, p + 0x114);
                }
                pos.x = pos.x - func_0200f5c8(func_02008444()) - 0x80000;
                pos.y = -pos.y + func_0200f5e8(func_02008444()) + 0x60000;
                if (pos.x > 0x98000 || pos.x < -0x98000 || pos.y > 0x78000 || pos.y < -0x78000) {
                    continue;
                }
                if (func_02058b4c((*(struct Entry0205d0d4 **)(p + 0x6c))[i].obj) != 0) {
                    func_02056b18(*(int *)(p + 0x80), 0x19f);
                    func_02056b18(*(int *)(p + 0x80) + 0xc8, 0x77df);
                }
                else if (func_02058b64((*(struct Entry0205d0d4 **)(p + 0x6c))[i].obj) != 0) {
                    func_02056b18(*(int *)(p + 0x80), 0x17bf);
                    func_02056b18(*(int *)(p + 0x80) + 0xc8, 0x77ff);
                }
                else {
                    func_02056b18(*(int *)(p + 0x80), 0x7e60);
                    func_02056b18(*(int *)(p + 0x80) + 0xc8, 0x76d6);
                }
                e = (*(struct Entry0205d0d4 **)(p + 0x6c))[i].obj;
                if (e == *(int *)(p + 0x68) &&
                    *(short *)(p + 0x58) <= 0x3c) {
                    r = (*(Obj0205d0d4 **)(p + 0x80))->slot36();
                    v = (r * *(short *)(p + 0x58)) / 0x3c;
                    (*(Obj0205d0d4 **)(p + 0x80))->slot35(v);
                    (*(Obj0205d0d4 **)(p + 0x80))->slot13(pos.x, pos.y, 0);
                    (*(Obj0205d0d4 **)(p + 0x80))->slot32();
                    (*(Obj0205d0d4 **)(p + 0x80))->slot35(r);
                }
                else {
                    (*(Obj0205d0d4 **)(p + 0x80))->slot13(pos.x, pos.y, 0);
                    (*(Obj0205d0d4 **)(p + 0x80))->slot32();
                }
                e = (*(struct Entry0205d0d4 **)(p + 0x6c))[i].obj;
                if (e == *(int *)(p + 0x68) &&
                    *(short *)(p + 0x58) <= 0x3c) {
                    r = ((Obj0205d0d4 *)(*(unsigned char **)(p + 0x80) + 0xc8))->slot36();
                    v = (r * *(short *)(p + 0x58)) / 0x3c;
                    ((Obj0205d0d4 *)(*(unsigned char **)(p + 0x80) + 0xc8))->slot35(v);
                    ((Obj0205d0d4 *)(*(unsigned char **)(p + 0x80) + 0xc8))->slot13(pos.x, pos.y, 0);
                    ((Obj0205d0d4 *)(*(unsigned char **)(p + 0x80) + 0xc8))->slot32();
                    ((Obj0205d0d4 *)(*(unsigned char **)(p + 0x80) + 0xc8))->slot35(r);
                }
                else {
                    ((Obj0205d0d4 *)(*(unsigned char **)(p + 0x80) + 0xc8))->slot13(pos.x, pos.y, 0);
                    ((Obj0205d0d4 *)(*(unsigned char **)(p + 0x80) + 0xc8))->slot32();
                }
            }
        }
    }
}
}
