struct Scene {
    char pad0[4];
    unsigned char state;
    char pad5[3];
    int mode;
};

struct Fade {
    char pad00[0x26];
    short f26;
};

extern char data_02099d78[];
extern char data_02099d58[];
extern char data_02099d88[];
extern char data_020cedc4[];
extern char data_02099da4[];
extern char data_02099db0[];
extern char data_02099dbc[];
extern struct Fade data_020c9ec8;
extern char data_02099dc8[];
extern char data_020cee88[];
extern char data_02099dd4[];
extern char data_02099ddc[];
extern char data_020cd7b4[];
extern void *func_02008444(void);
extern void func_0200fe38(void *game, int value);
extern void func_0201192c(void *game, int value);
extern void func_02011cdc(void *game, int value);
extern void func_02010384(void *game, int value);
extern void func_020322b4(void *game);
extern char *func_02015a0c(void *game);
extern int func_020790c8(const char *a, const char *b);
extern void func_02010298(void *game, char *name);
extern void func_02051ca4(char *name);
extern int func_020322c4(void *game);
extern void func_02031fe8(void *game);
extern void func_02011478(void *game, int value);
extern void func_02011ca8(void *game);
extern void *func_020507c4(void *heap);
extern void *func_02009900(void);
extern void func_0202f7e8(void *list, void *object, int a, int b);
extern int func_02027518(int id);
extern void func_020114dc(void *game, int value);
extern void func_02011584(void *game);
extern int func_02031fb8(void *game);
extern void func_02031ff8(void *game);
extern int func_020322d4(void *game);
extern void func_02027528(int id);
extern int func_02031fd0(void *game);
extern void func_020322e4(void *game);
extern int func_0201fa54(void *game);
extern void func_0201153c(void *game, int value);
extern void func_020125a8(void *game, int value);
extern int func_020112a8(void *game);
extern void *func_02022dfc(struct Scene *scene);
extern void func_02022f34(void *object, char *name, int value);
extern void func_020521d4(char *name, int a, int b, int c);
extern void func_02027538(int id);
extern int func_0202fa8c(void *list, char *name);
extern int func_02012518(void *game, char *a, char *b);
extern int func_0201572c(void);
extern void func_02011454(void *game);
extern void func_020322f4(void *game);
extern void func_02010074(void *game, int value);
extern void func_020100a0(void *game, int value);
extern void *func_02031b20(struct Scene *scene);
extern void func_02022f2c(void *object, char *name, int value);
extern void func_0201139c(void *game);

void func_02032018(struct Scene *scene)
{
    void *object;
    void *game;

    func_0200fe38(func_02008444(), 0);
    switch (scene->state) {
    case 0:
        scene->state = 1;
        break;
    case 1:
        func_0201192c(func_02008444(), 1);
        func_02011cdc(func_02008444(), 0);
        func_02010384(func_02008444(), 0);
        func_020322b4(func_02008444());
        if (func_020790c8(func_02015a0c(func_02008444()), data_02099d78) == 0) {
            func_02010298(func_02008444(), data_02099d58);
            func_02051ca4(data_02099d88);
        } else if (func_020322c4(func_02008444()) != 0) {
            func_02031fe8(func_02008444());
            func_02011478(func_02008444(), 0);
            func_02011ca8(func_02008444());
            object = func_020507c4(data_020cedc4);
            if (object != 0) {
                func_0202f7e8(func_02009900(), object, 4, 0);
            }
        } else if (func_02027518(0x1003) != 0) {
            func_020114dc(func_02008444(), 0);
            func_02011584(func_02008444());
            func_02051ca4(func_02015a0c(func_02008444()));
        } else if (func_02031fb8(func_02008444()) != 0) {
            func_02031ff8(func_02008444());
            if (func_020322d4(func_02008444()) != 0) {
                func_02011478(func_02008444(), 0);
            }
            func_02011ca8(func_02008444());
            func_02011584(func_02008444());
            func_02051ca4(func_02015a0c(func_02008444()));
            scene->mode = 1;
            func_02027528(0x1004);
        } else if (func_02031fd0(func_02008444()) != 0) {
            func_020322e4(func_02008444());
            game = func_02008444();
            func_0201153c(game, func_0201fa54(func_02008444()));
            game = func_02008444();
            func_020125a8(game, func_0201fa54(func_02008444()));
            func_02051ca4(data_02099da4);
            func_02011584(func_02008444());
            scene->mode = 2;
            func_02027528(0x1005);
        } else {
            func_020114dc(func_02008444(), 0);
            func_02011584(func_02008444());
            func_02051ca4(data_02099db0);
        }
        if (func_020112a8(func_02008444()) == 0) {
            func_02022f34(func_02022dfc(scene), data_02099dbc, 0);
        }
        if (data_020c9ec8.f26 == -0x10) {
            func_020521d4(data_02099dc8, 1, 0xf, 0x100);
        }
        func_02027538(0x1003);
        scene->state = 2;
        break;
    case 2:
        if (func_0202fa8c(func_02009900(), data_020cee88) != 0 && func_02012518(func_02008444(), data_02099dd4, data_02099ddc) != 0) {
            scene->state = 3;
        }
        break;
    case 3:
        if (func_0201572c() == 0) {
            func_02011454(func_02008444());
            func_020322f4(func_02008444());
            func_02010074(func_02008444(), 0);
            func_020100a0(func_02008444(), 0);
            func_02022f2c(func_02031b20(scene), data_020cd7b4, scene->mode);
            scene->state = 5;
        }
        break;
    case 5:
        func_0200fe38(func_02008444(), 1);
        func_0201139c(func_02008444());
        break;
    case 6:
        break;
    }
}
