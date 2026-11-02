struct Panel {
    char pad00[5];
    unsigned char b05;
    char pad06[0x1e];
    void *f24;
};

extern char data_02099f90[];
extern char data_020cd96c[];
extern char data_020cd7ec[];
extern char data_02099f98[];
extern char data_02099fa4[];
extern char data_02099fb0[];
extern char data_02099fc8[];
extern char data_02099fd4[];
extern char data_020cd7d0[];
extern char data_02099fe0[];
extern char data_02099fec[];
extern char data_02099ff8[];
extern char data_0209a008[];
extern char data_0209a014[];
extern char data_0209a024[];

extern signed char *func_02013514(int id);
extern signed char *func_02078fbc(signed char *text, char *part);
extern int func_020790c8(signed char *a, char *b);
extern void func_020135b8(void *window, unsigned char value);
extern void func_02010384(void *window, int value);
extern void func_02010ef4(void *window, int a, int b);
extern void func_020112d4(void *window);
extern void func_02011598(void *window);
extern void *func_02008444(void);
extern int func_0201132c(void *game);
extern void *func_02011264(void);
extern void *func_0201123c(void *list, int index);
extern struct Panel *func_02022fc0(void *list, char *name);
extern void func_02061138(void *object, int value);
extern void func_020337e0(struct Panel *panel, int value);
extern void func_020337e8(struct Panel *panel, int value);
extern void func_02010f0c(void *window, int a, int b);
extern void func_02010e7c(void *window);
extern void func_02010e5c(void *window);
extern int func_02010f84(void *window, int a);
extern void func_02022f2c(void *list, char *name, int value);
extern void func_0201192c(void *window, int value);
extern int func_02022fa4(void *list, char *name);
extern void func_020337f0(struct Panel *panel);
extern void func_020337f8(struct Panel *panel);

void func_02033504(void *context, void *window, int id)
{
    signed char *name;
    struct Panel *panel;

    name = func_02013514(id);
    if (func_02078fbc(name, data_02099f90) != 0) {
        func_020135b8(window, name[0] != 'S');
        func_02010384(window, 1);
        func_02010ef4(window, 0xff, -1);
        func_020112d4(window);
        func_02011598(window);
        return;
    }
    if (func_0201132c(func_02008444()) != 0) {
        panel = func_02022fc0(func_0201123c(func_02011264(), 0), data_020cd96c);
    } else {
        panel = func_02022fc0(func_0201123c(func_02011264(), 0), data_020cd7ec);
    }
    if (panel == 0) {
        return;
    }
    if (func_020790c8(name, data_02099f98) == 0) {
        if (panel->f24 != 0) {
            func_02061138(panel->f24, 0);
        }
        func_02010ef4(window, 0xff, -1);
        func_020337e0(panel, 1);
        return;
    }
    if (func_020790c8(name, data_02099fa4) == 0) {
        if (panel->f24 != 0) {
            func_02061138(panel->f24, 1);
        }
        func_020337e8(panel, 0);
        func_020337e0(panel, 0);
        func_02010f0c(window, 2, 3);
        func_02010f0c(window, 1, 3);
        if (func_0201132c(func_02008444()) != 0) {
            func_02010e7c(window);
        } else {
            func_02010e5c(window);
        }
        if (func_02010f84(window, 6) != 0) {
            if (func_0201132c(func_02008444()) != 0) {
                func_02010f0c(window, 7, 3);
            } else {
                func_02010f0c(window, 6, 3);
            }
        }
        return;
    }
    if (func_020790c8(name, data_02099fb0) == 0) {
        if (panel->f24 != 0) {
            func_02061138(panel->f24, 1);
        }
        func_020337e8(panel, 0);
        func_020337e0(panel, 0);
        func_02010f0c(window, 2, 5);
        func_02010f0c(window, 1, 5);
        if (func_02010f84(window, 6) != 0) {
            if (func_0201132c(func_02008444()) != 0) {
                func_02010f0c(window, 7, 5);
            } else {
                func_02010f0c(window, 6, 5);
            }
        }
        return;
    }
    if (func_020790c8(name, data_02099fc8) == 0) {
        func_020337e8(panel, 1);
        return;
    }
    if (func_020790c8(name, data_02099fd4) == 0) {
        func_02010f0c(window, 2, 5);
        func_02010f0c(window, 1, 5);
        func_02022f2c(func_0201123c(func_02011264(), 0), data_020cd7d0, 0);
        func_0201192c(window, 3);
        return;
    }
    if (func_020790c8(name, data_02099fe0) == 0) {
        func_02010f0c(window, 2, 5);
        func_02010f0c(window, 1, 5);
        func_02022f2c(func_0201123c(func_02011264(), 0), data_020cd7d0, 0);
        func_02011598(window);
        func_0201192c(window, 2);
        return;
    }
    if (func_020790c8(name, data_02099fec) == 0) {
        panel->b05 = 0;
        if (func_02022fa4(func_0201123c(func_02011264(), 1), data_02099ff8) != 0) {
            func_02011598(window);
        }
        return;
    }
    if (func_020790c8(name, data_0209a008) == 0) {
        panel->b05 = 1;
        return;
    }
    if (func_020790c8(name, data_0209a014) == 0) {
        func_020337f0(panel);
        return;
    }
    if (func_020790c8(name, data_0209a024) == 0) {
        func_020337f8(panel);
    }
}
