struct Part {
    char data[0x44];
};

struct Iter {
    int words[4];
};

class Scene {
public:
    virtual void s0();
    virtual void s1();
    virtual void s2();
    virtual void s3();
    virtual void s4();
    virtual void s5();
    virtual void s6();
    virtual void s7();
    virtual void s8();
    virtual void s9();
    virtual void s10();
    virtual void s11();
    virtual void s12();
    virtual void s13();
    virtual void s14();
    virtual void s15();
    virtual void s16();
    virtual void s17();
    virtual void s18();
    virtual void s19();
    virtual void s20();
    virtual void s21();
    virtual void s22();
    virtual void s23();
    virtual void s24();
    virtual void s25();
    virtual void s26();
    virtual void s27();
    virtual void s28();
    virtual void s29();
    virtual void s30();
    virtual void s31();
    virtual void s32();
    virtual void s33(int a);
    char pad4[0x44];
    int count;
    int index;
    int pad50;
    struct Part *parts;
};

extern "C" {
extern char data_020cf860[];
extern char data_020cf388[];
extern void func_0202fbcc(Iter *it, int kind);
extern int func_0200eac4(Iter *it);
extern void *func_0200eab4(Iter *it);
extern void *func_0200eab8(Iter *it);
extern void func_0200eabc(Iter *it, int step);
extern int func_02008828(void *object, char *type);
extern struct Part *func_0207dc50(int count, int size, int align, void *ctor, void *dtor);
extern struct Part *func_02028b28(struct Part *part);
extern char *func_02028b08(char *part);
extern void *func_02059b88(void *object);
extern int func_02008350(void *object);
extern void func_02029ab8(void *model);
extern void func_02028ba8(struct Part *part, void *model);
extern void func_02028c34(struct Part *part, void *object, int id, int a, int b);
extern void func_02029780(struct Part *part, int value);
void func_0205c668(Scene *scene);
}

void func_0205c668(Scene *scene)
{
    Iter it;
    Iter it2;
    void *object;
    void *model;
    int id;

    func_0202fbcc(&it, 6);
    while (func_0200eac4(&it) != 0) {
        if (func_02008828(func_0200eab4(&it), data_020cf860) != 0) {
            scene->count++;
        }
        func_0200eabc(&it, 0);
    }
    scene->parts = func_0207dc50(scene->count, 0x44, 8, (void *)func_02028b08, (void *)func_02028b28);
    func_0202fbcc(&it2, 6);
    while (func_0200eac4(&it2) != 0) {
        if (func_02008828(func_0200eab4(&it2), data_020cf860) != 0 && func_02008828(func_0200eab4(&it2), data_020cf388) == 0) {
            object = func_0200eab8(&it2);
            model = func_02059b88(object);
            id = func_02008350(object);
            if (model == 0) {
                func_02029780(&scene->parts[scene->index++], 0);
            } else {
                func_02029ab8(model);
                func_02028ba8(&scene->parts[scene->index], model);
                func_02028c34(&scene->parts[scene->index], object, id, 0, 1);
                func_02029780(&scene->parts[scene->index++], 1);
            }
        }
        func_0200eabc(&it2, 0);
    }
    scene->s33(0xa80);
}
