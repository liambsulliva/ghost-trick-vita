typedef int code();

struct Shared {
    unsigned char hidden;
    unsigned char alt;
    char pad02[6];
    int timer;
};

struct Scene {
    char pad00[0x38];
    int *meter;
    int *badge;
    int *top;
    int *bottom;
    int *icon_b;
    int *icon_a;
    char pad50[7];
    unsigned char mode;
};

extern struct Shared data_020cdd0c;
extern int func_02031254();
extern int func_020380b4();
extern int func_0202f624();
extern int func_0203aedc();
extern int func_02037ca4();
extern int func_0203b1b8();
extern int func_02037cac();
extern int func_02037cb8();
extern int func_02009900();
extern int func_0202f7e8();
extern int func_02008444();
extern int func_020322d4();
extern int func_0203a104();
extern int func_02037cc8();
extern int func_02037cd4();
extern int func_02038150();
extern int func_02037094();

void func_02037ac8(struct Scene *scene)
{
    int *p;
    int value;

    p = (int *)func_02031254(8);
    if (p != 0) {
        p = (int *)func_020380b4(p);
    }
    data_020cdd0c.timer = (int)p;
    p = (int *)func_0202f624(0xf0);
    if (p != 0) {
        p = (int *)func_0203aedc(p, 1);
    }
    scene->top = p;
    if (p != 0) {
        func_02037ca4(p, 6);
        (**(code **)(*scene->top + 0x28))(scene->top);
        func_0203b1b8(scene->top, 0xfff90000, 0x48000, 0xfff68000, 0x48000);
        func_02037cac(scene->top, 0, 0xc);
        func_02037cb8(scene->top);
        func_0202f7e8(func_02009900(), scene->top, 7, 0);
    }
    p = (int *)func_0202f624(0xf0);
    if (p != 0) {
        p = (int *)func_0203aedc(p, 2);
    }
    scene->bottom = p;
    if (p != 0) {
        func_02037ca4(p, 6);
        (**(code **)(*scene->bottom + 0x28))(scene->bottom);
        func_0203b1b8(scene->bottom, 0xfff90000, 0xfffce000, 0xfff68000, 0xfffce000);
        func_02037cac(scene->bottom, 0, 0x86);
        func_02037cb8(scene->bottom);
        func_0202f7e8(func_02009900(), scene->bottom, 7, 0);
    }
    if (data_020cdd0c.hidden == 0 && func_020322d4(func_02008444()) == 0) {
        p = (int *)func_0202f624(0xdc);
        if (p != 0) {
            p = (int *)func_0203a104(p);
        }
        scene->badge = p;
        if (p != 0) {
            func_02037cc8(p, 0x800a, 4);
            func_02037cd4(scene->badge, 5, 0xaa);
            (**(code **)(*scene->badge + 0x28))(scene->badge);
            func_0202f7e8(func_02009900(), scene->badge, 7, 0);
        }
    }
    value = -1;
    if (data_020cdd0c.alt != 0) {
        value = 0x16;
    }
    p = (int *)func_02031254(0x38);
    if (p != 0) {
        p = (int *)func_02038150(p, scene->mode, value);
    }
    scene->meter = p;
    p = (int *)func_0202f624(0xcc);
    if (p != 0) {
        p = (int *)func_02037094(p);
    }
    scene->icon_a = p;
    if (p != 0) {
        value = 0x800e;
        if (scene->mode != 0 || data_020cdd0c.alt != 0) {
            value = 0x8010;
        } else if (func_020322d4(func_02008444()) != 0) {
            value = 0x800f;
        }
        func_02037cc8(scene->icon_a, value, 4);
        (**(code **)(*scene->icon_a + 0x28))(scene->icon_a);
        func_0202f7e8(func_02009900(), scene->icon_a, 7, 0);
    }
    p = (int *)func_0202f624(0xcc);
    if (p != 0) {
        p = (int *)func_02037094(p);
    }
    scene->icon_b = p;
    if (p != 0) {
        func_02037cc8(p, 0x8009, 4);
        (**(code **)(*scene->icon_b + 0x28))(scene->icon_b);
        func_0202f7e8(func_02009900(), scene->icon_b, 7, 0);
    }
}
