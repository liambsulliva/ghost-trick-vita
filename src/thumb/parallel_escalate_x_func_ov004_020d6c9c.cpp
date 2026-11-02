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
    virtual int s13();
    virtual void s14();
};

struct SceneData {
    char pad0[4];
    unsigned char state;
    unsigned char timer;
    unsigned char wait;
    char pad7[0x1d];
    unsigned char found;
};

struct Flags {
    unsigned int bits;
};

struct Sound {
    char pad0[0x12];
    short level;
};

struct Display {
    char pad0[0x15];
    unsigned char mode;
};

struct Keys {
    unsigned int held;
    unsigned int pressed;
};

extern "C" {
extern char data_020cfd28[];
extern char data_020cfd0c[];
extern char data_ov004_020e45f8[];
extern struct Flags data_020c9e48;
extern struct Sound data_020c9e88;
extern struct Display data_020cff60;
extern struct Keys data_020c9d50;
extern void *func_02009900(void);
extern void *func_0202fb48(void *manager, char *type, int index);
extern int func_02049ab8(void *object);
extern void *func_0202f25c(void);
extern void func_ov004_020d6e90(void *system, int value);
extern void func_ov004_020d6ea8(void *system, int value);
extern int func_02061bd4(void *system);
extern void func_02023bb4(char *name);
extern int func_0201572c(void);
extern void func_0202fad4(void *manager, char *type);
extern unsigned int func_ov004_020d6f34(struct SceneData *scene);
extern void *func_02010b7c(void);
extern int func_02015154(void *game);
extern void *func_0200847c(void);
extern void func_0202a16c(void *sound, int id);
extern int func_0202f24c(void *system);
extern void func_ov004_020d5958(void *system);
extern void func_02022f6c(struct SceneData *scene);
void func_ov004_020d6c9c(Scene *scene);
}

void func_ov004_020d6c9c(Scene *scene)
{
    struct SceneData *s;
    void *object;

    s = (struct SceneData *)scene;
    switch (s->state) {
    case 0xff:
        s->state = 0;
        break;
    case 0:
        object = func_0202fb48(func_02009900(), data_020cfd28, -1);
        if (object == 0 || (object != 0 && func_02049ab8(object) != 0)) {
            data_020c9e48.bits &= ~0x1800;
            func_ov004_020d6e90(func_0202f25c(), 1);
            func_ov004_020d6ea8(func_0202f25c(), 1);
            scene->s11();
            s->found = object != 0;
            s->state++;
        }
        break;
    case 1:
        if (func_02061bd4(func_0202f25c()) != 0) {
            func_02023bb4(data_ov004_020e45f8);
            if (func_0201572c() == 0 && data_020c9e88.level != 0) {
                data_020c9e88.level = 0;
            }
            if (s->found == 0) {
                data_020cff60.mode |= 0x40;
                func_0202fad4(func_02009900(), data_020cfd0c);
                data_020c9e48.bits &= ~0x100;
                s->state = 99;
                break;
            }
            s->state++;
            s->timer = 6;
        }
        break;
    case 2:
        s->timer--;
        if (s->timer == 0) {
            data_020cff60.mode |= 0x40;
            func_0202fad4(func_02009900(), data_020cfd0c);
            data_020c9e48.bits &= ~0x100;
            s->state++;
        }
        break;
    default:
        if (s->wait != 0) {
            s->wait--;
        } else if (scene->s13() != 0 && func_ov004_020d6f34(s) > 30
            && ((data_020c9d50.pressed & 0x2000) != 0 || func_02015154(func_02010b7c()) != 0)) {
            func_0202a16c(func_0200847c(), 0x5a);
            scene->s14();
            s->state = 200;
            break;
        }
        if (func_0202f24c(func_0202f25c()) == 0) {
            func_ov004_020d5958(func_0202f25c());
            scene->s12();
            func_02022f6c(s);
        }
        break;
    case 200:
        if (func_0201572c() == 0) {
            func_ov004_020d5958(func_0202f25c());
            scene->s12();
            s->state++;
        }
        break;
    case 201:
        if (func_0202f24c(func_0202f25c()) == 0) {
            func_02022f6c(s);
        }
        break;
    }
}
