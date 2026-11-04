class Table {
public:
    virtual void v00();
    virtual void v04();
    virtual void v08();
    virtual void v0c();
    virtual void v10();
    virtual void v14();
    virtual int v18(unsigned int index);
};

typedef int code(...);

struct Scene {
    void *f00;
    int *f04;
    int *f08;
    void *f0c;
    void *f10;
    Table *table;
    int f18;
    int f1c;
    char pad20[4];
    short f24;
    char pad26[0xc];
    unsigned char f32;
};

extern "C" {
extern void *func_02008444(void);
extern unsigned int func_02038a84(void *object);
extern Table *func_0201ff10(void *a, int b);
extern int func_0201f944(Table *a, unsigned char b);
extern void func_0201f974(Table *a, unsigned char b, unsigned char c, int d);
extern int func_0201fbc8(Table *a);
extern int func_020322d4(void *game);
extern int func_0203853c(struct Scene *scene);
extern void *func_02031254(int size);
extern void *func_02039c2c(void *p, short a, unsigned char b, int c);
extern int *func_0202f624(int size);
extern int *func_02038a90(int *p);
extern void func_0205765c(int *p, int a, int b, int c, int d);
extern void *func_02009900(void);
extern void func_0202f7e8(void *a, int *b, int c, int d);
extern int *func_0203706c(int *p);
extern void *func_0203a400(void *p, struct Scene *scene);
extern void func_0203a414(void *p);
extern void *func_02038ad4(void *p);
extern void func_02038ac4(void *p);
extern void func_02038acc(void *p, int v);
extern void *func_02010b7c(void);
extern void func_0202f174(void *a, void *b);
extern void func_02038ad0(void *p, struct Scene *scene);
void func_02038848(struct Scene *scene, int fresh);
}

void func_02038848(struct Scene *scene, int fresh)
{
    void *p;
    int *obj;

    if (scene->f24 == -1) {
        if (func_02038a84(func_02008444()) != 0) {
            scene->f24 = func_02038a84(func_02008444());
        }
    }
    scene->table = func_0201ff10(func_02008444(), 0);
    if (scene->table != 0) {
        if (func_0201f944(scene->table, 1) < 0) {
            func_0201f974(scene->table, 1, 0, -1);
        }
        if (scene->f24 != -1) {
            func_0201f974(scene->table, scene->f24, 0, -1);
        }
        scene->f18 = func_0201fbc8(scene->table);
        if (scene->f24 != -1) {
            scene->f1c = scene->table->v18(scene->f24);
        } else {
            scene->f1c = scene->table->v18(1);
        }
    }
    scene->f32 = func_020322d4(func_02008444());
    if (fresh != 0) {
        scene->f1c = func_0203853c(scene) + 1;
        scene->f32 = 0;
    }
    if (scene->f32 != 0) {
        scene->f24 = -1;
        scene->f1c = 0;
        scene->f18 = 2;
    }
    p = func_02031254(0x14);
    if (p != 0) {
        p = func_02039c2c(p, scene->f1c, scene->f32, fresh);
    }
    scene->f00 = p;
    obj = func_0202f624(0xd4);
    if (obj != 0) {
        obj = func_02038a90(obj);
    }
    scene->f04 = obj;
    if (obj != 0) {
        (**(code **)(*obj + 0xa8))(obj, 0x58, 0, 1, 0, 1, -1, 0);
        (**(code **)(*scene->f04 + 0x84))(scene->f04, 0x430);
        func_0205765c(scene->f04, 0, 0, 0xc0000, 0x28000);
        (**(code **)(*scene->f04 + 0xb8))(scene->f04, 0xc0000, 0x28000);
        (**(code **)(*scene->f04 + 0x34))(scene->f04, -0x20000, 0xc000, 0);
        func_0202f7e8(func_02009900(), scene->f04, 1, 0);
    }
    obj = func_0202f624(0xd0);
    if (obj != 0) {
        obj = func_0203706c(obj);
    }
    scene->f08 = obj;
    if (obj != 0) {
        (**(code **)(*obj + 0xa8))(obj, 0x5a, 0, 1, 0, 1, -1, 0);
        (**(code **)(*scene->f08 + 0x84))(scene->f08, 0x440);
        func_0205765c(scene->f08, 0, 0, 0x100000, 0x10000);
        (**(code **)(*scene->f08 + 0xb8))(scene->f08, 0x100000, 0x10000);
        (**(code **)(*scene->f08 + 0x34))(scene->f08, 0, 0xc000, 0);
        (**(code **)(*scene->f08 + 0x2c))(scene->f08);
        func_0202f7e8(func_02009900(), scene->f08, 1, 0);
    }
    p = func_02031254(8);
    if (p != 0) {
        p = func_0203a400(p, scene);
    }
    scene->f0c = p;
    if (p != 0) {
        func_0203a414(p);
    }
    p = func_02031254(0x10);
    if (p != 0) {
        p = func_02038ad4(p);
    }
    scene->f10 = p;
    if (p != 0) {
        func_02038ac4(p);
        func_02038acc(scene->f10, 2);
        func_0202f174(func_02010b7c(), scene->f10);
        func_02038ad0(scene->f10, scene);
    }
}
