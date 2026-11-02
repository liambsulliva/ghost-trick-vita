struct Scene02031e30 {
    char pad0[8];
    int state;
};

extern char data_020cf75c[];
extern char data_020c9e88[];
extern char data_02099d38[];
extern char data_02099d44[];
extern char data_02099d50[];
extern char data_02099d58[];
extern char data_02099d5c[];
extern char data_02099d6c[];

extern void *func_02008860(void);
extern void func_0200c82c(void *object, int index);
extern void *func_02009900(void);
extern void func_0202fad4(void *object, void *data);
extern void *func_02008444(void);
extern int func_02031fb8(void *object);
extern void func_0200ec24(void *object, int value);
extern void func_02010dd0(void *object);
extern void func_0200fbc8(void *object);
extern char *func_02015a0c(void *object);
extern int func_020790c8(const char *a, const char *b);
extern void func_020112b4(void *object);
extern void func_020521d4(void *object, int index, int a, int b);
extern void *func_02031b20(struct Scene02031e30 *scene);
extern void func_02022f34(void *object, void *data, int value);
extern void *func_02011264(void);
extern void *func_0201123c(void *list, int index);
extern void func_02022f4c(void *object);
extern void func_02010298(void *object, void *data);
extern int func_02027518(int id);
extern void func_02027538(int id);
extern int func_02031fd0(void *object);
extern void func_02031fe8(void *object);
extern void func_02031ff8(void *object);
extern void func_02032008(void *object);
extern void func_020125d4(void *object, int value);

void func_02031e30(struct Scene02031e30 *scene)
{
    func_0200c82c(func_02008860(), 0);
    func_0200c82c(func_02008860(), 1);
    func_0202fad4(func_02009900(), data_020cf75c);
    if (func_02031fb8(func_02008444()) == 0) {
        func_0200ec24(func_02008444(), 0);
    }
    if (*(short *)(data_020c9e88 + 0x12) == 0) {
        *(short *)(data_020c9e88 + 0x12) = -0x10;
    }
    func_02010dd0(func_02008444());
    func_0200fbc8(func_02008444());
    scene->state = 0;
    if (func_020790c8(func_02015a0c(func_02008444()), data_02099d38) == 0) {
        func_020112b4(func_02008444());
        func_020521d4(data_02099d44, 0, 0, 0x100);
        func_020521d4(data_02099d44, 1, 0, 0x100);
        func_02022f34(func_02031b20(scene), data_02099d50, 0);
        func_02022f4c(func_0201123c(func_02011264(), 1));
        func_02010298(func_02008444(), data_02099d58);
        return;
    }
    if (func_020790c8(func_02015a0c(func_02008444()), data_02099d5c) == 0) {
        func_020112b4(func_02008444());
        func_020521d4(data_02099d44, 0, 0, 0x100);
        func_020521d4(data_02099d44, 1, 0, 0x100);
        func_02022f34(func_02031b20(scene), data_02099d6c, 0);
        func_02022f4c(func_0201123c(func_02011264(), 1));
        func_02010298(func_02008444(), data_02099d58);
        return;
    }
    if (func_02027518(0x1002) != 0) {
        func_02027538(0x1002);
        if (func_02031fd0(func_02008444()) == 0) {
            func_02027538(0x1003);
            func_02031fe8(func_02008444());
            func_02031ff8(func_02008444());
            func_02032008(func_02008444());
            func_020125d4(func_02008444(), 0);
        }
    }
}
