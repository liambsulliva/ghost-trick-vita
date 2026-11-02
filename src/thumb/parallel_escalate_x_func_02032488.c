typedef struct Scene32488 {
    char pad0[4];
    unsigned char wait;
    char pad5[0x1b];
    unsigned char x;
    char pad21;
    unsigned char y;
    unsigned char room;
} Scene32488;

extern char data_020cfed4;
extern char data_020cf7c8;
extern char data_02099e60;
extern char data_02099e6c;
extern char data_02099dfc;
extern char data_02099e00;
extern char data_02099e74;
extern void *func_02008444(void);
extern void *func_02009900(void);
extern int func_02010b38(void *game);
extern void func_02032644(void *game, int mode);
extern void func_0200fe38(void *game, int value);
extern int func_0200f330(void *game);
extern void func_020109c8(void *game, int x, int y);
extern int func_020283b0(void *game);
extern int func_0202fa8c(void *manager, void *type);
extern int func_020322d4(void *game);
extern void *func_0202fb48(void *manager, void *type, int index);
extern void func_02032650(void *object, int room);
extern void *func_02057b04(void *type);
extern void func_02057d34(void *object, int from, int to);
extern void func_0202f7e8(void *manager, void *object, int a, int b);
extern int func_02011320(void *game);
extern int func_0201260c(void *game);
extern void *func_0203247c(void *game);
extern void func_0201192c(void *game, void *value);
extern void func_02010384(void *game, int value);
extern int func_02012518(void *game, void *a, void *b);
extern void func_0200da04(void *object, void *a, void *b, void *c);
extern void *func_02031b20(Scene32488 *scene);
extern void func_02022f34(void *object, void *table, int index);

void func_02032488(Scene32488 *scene)
{
    unsigned char room;
    void *object;
    void *game;

    if (scene->wait != 0) {
        if (scene->wait > 4) {
            scene->wait--;
            return;
        }
        if (func_02010b38(func_02008444()) == 0) {
            func_02032644(func_02008444(), 1);
            scene->wait--;
        }
        if (scene->wait != 0) {
            return;
        }
        func_02032644(func_02008444(), 4);
    }
    func_0200fe38(func_02008444(), 0);
    if (func_0200f330(func_02008444()) == 0) {
        func_020109c8(func_02008444(), scene->x, scene->y);
    }
    if (func_02010b38(func_02008444()) != 0) {
        return;
    }
    room = scene->room;
    if (room != func_020283b0(func_02008444())) {
        scene->room = func_020283b0(func_02008444());
        if (func_0202fa8c(func_02009900(), &data_020cfed4) == 0 && func_020322d4(func_02008444()) != 0) {
            object = func_0202fb48(func_02009900(), &data_020cf7c8, -1);
            if (object != 0) {
                func_02032650(object, scene->room);
            } else {
                object = func_02057b04(&data_020cf7c8);
                if (object != 0) {
                    func_02057d34(object, room, scene->room);
                    func_0202f7e8(func_02009900(), object, 7, 0);
                }
            }
        }
    }
    switch (func_02011320(func_02008444())) {
    case 0:
        break;
    case 12:
        if (func_0201260c(func_02008444()) == 0) {
            game = func_02008444();
            func_0201192c(game, func_0203247c(func_02008444()));
            func_02010384(func_02008444(), 0);
            if (func_02012518(func_02008444(), &data_02099e60, &data_02099e6c) != 0) {
                func_0200da04(&data_02099dfc, &data_02099e60, &data_02099e6c, &data_02099e00);
            }
            break;
        }
    case 9:
    case 10:
    case 11:
    case 13:
        object = func_02031b20(scene);
        func_02022f34(object, &data_02099e74, func_02011320(func_02008444()) - 9);
        func_02010384(func_02008444(), 1);
        break;
    }
}
