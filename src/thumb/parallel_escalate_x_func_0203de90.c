typedef struct Owner3de90 {
    char pad0[0x24];
    void *view;
} Owner3de90;

typedef struct Talk3de90 {
    char pad0[4];
    unsigned char state;
    char pad5[0x1b];
    Owner3de90 *owner;
    char pad24[4];
    int timer;
} Talk3de90;

typedef struct Keys3de90 {
    unsigned int held;
    unsigned int trigger;
} Keys3de90;

extern Keys3de90 data_020c9d50;
extern char data_0209abe4;
extern char data_0209abe8;
extern char data_0209abf0;
extern char data_0209abf8;
extern char data_0209ac00;
extern char data_0209ac08;
extern char data_0209ac10;
extern char data_0209ac28;
extern char data_0209ac30;
extern char data_0209ac38;
extern char data_0209ac40;
extern void *func_02008444(void);
extern void func_0200da04(void *a, void *b, void *c, void *d);
extern void func_0201192c(void *game, int value);
extern int func_02061044(void *view);
extern void func_02061138(void *view, int value);
extern int func_0203247c(void *game);
extern int func_02010b38(void *game);
extern void *func_02031b20(Talk3de90 *talk);
extern void func_02022f34(void *object, void *table, int index);
extern void func_02010ef4(void *game, int a, int b);
extern void *func_02011264(void);
extern void *func_0201123c(void *object, int index);
extern void func_02022f6c(Talk3de90 *talk);

void func_0203de90(Talk3de90 *talk)
{
    void *game;

    switch (talk->state) {
    case 0:
        talk->timer--;
        if (talk->timer == 0) {
            func_0200da04(&data_0209abe4, &data_0209abe8, &data_0209abf0, &data_0209abf8);
            talk->state = 99;
        }
        break;
    case 1:
        func_0201192c(func_02008444(), 4);
        if (func_02061044(talk->owner->view) != 0 || (data_020c9d50.held & 0xf00) != 0) {
            func_0200da04(&data_0209abe4, &data_0209abe8, &data_0209ac00, &data_0209abf8);
            func_02061138(talk->owner->view, 0);
            game = func_02008444();
            func_0201192c(game, func_0203247c(func_02008444()));
            talk->state = 99;
        }
        break;
    case 2:
        if (func_02061044(talk->owner->view) != 0 || (data_020c9d50.held & 0xf00) != 0) {
            func_0200da04(&data_0209abe4, &data_0209abe8, &data_0209ac08, &data_0209abf8);
        }
        break;
    case 10:
        if (func_02010b38(func_02008444()) == 0 && (data_020c9d50.trigger & 0xa000) != 0) {
            func_02022f34(func_02031b20(talk), &data_0209ac10, 0);
            func_0201192c(func_02008444(), 3);
            func_02010ef4(func_02008444(), 2, -1);
        }
        break;
    case 20:
        func_0201192c(func_02008444(), 4);
        talk->state = 21;
    case 21:
        if (func_02061044(talk->owner->view) != 0 || (data_020c9d50.held & 0xf00) != 0) {
            func_0200da04(&data_0209abe4, &data_0209abe8, &data_0209ac28, &data_0209ac30);
            talk->state = 22;
        }
        break;
    case 22:
        if (func_02061044(talk->owner->view) != 0 || (data_020c9d50.held & 0xf00) != 0) {
            func_0200da04(&data_0209abe4, &data_0209abe8, &data_0209ac38, &data_0209ac30);
        } else {
            func_0200da04(&data_0209abe4, &data_0209abe8, &data_0209ac40, &data_0209ac30);
            talk->state = 21;
        }
        break;
    case 23:
        func_02061138(talk->owner->view, 0);
        game = func_02008444();
        func_0201192c(game, func_0203247c(func_02008444()));
        talk->state = 99;
        break;
    case 99:
        break;
    case 255:
        func_02022f34(func_0201123c(func_02011264(), 0), &data_0209ac10, 0);
        func_0201192c(func_02008444(), 3);
        break;
    default:
        func_02022f6c(talk);
        break;
    }
}
