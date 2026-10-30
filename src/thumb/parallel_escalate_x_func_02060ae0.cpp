class Self02060ae0 {
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
    virtual void slot13();
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
    virtual void slot35();
    virtual void slot36();
    virtual void slot37();
    virtual void slot38();
    virtual void slot39();
    virtual void slot40();
    virtual void slot41();
};

struct Point02060ae0 {
    int x;
    int y;
    int z;
};

extern "C" {
extern int func_02010b54(void *object);
extern unsigned int *func_02008444(void);
extern int func_0203435c(void *object);
extern unsigned short *func_0200eb78(void);
extern int func_02022ca4(void *owner, int value);
extern void func_02060cf4(int value);
extern void func_02060d00(int value);
extern void *func_02008eb0(void);
extern unsigned int func_01ff80b8(void *state);
extern unsigned char *func_0202970c(void);
extern int func_02029708(void *object);
extern void func_0206f77c(void *mtx);
extern void func_0206f7b4(void *mtx);
extern void func_02060d0c(int s, int t);
extern void func_02060d2c(int x, int y, int z);
extern void func_02060d48(void);
extern void func_0205f080(int value);
extern int data_0209dbd8[];
extern int data_0209db98[];
extern int data_020af554[];
extern char data_020af56c[];

void func_02060ae0(unsigned char *p)
{
    int negh;
    int count;
    short h;
    int i;
    int value;
    int right;
    unsigned char j;
    struct Point02060ae0 *pts;
    int x;
    int y;
    int s;
    unsigned char c;

    if (func_02010b54(p) == 0) {
        return;
    }
    if (func_0203435c(func_02008444()) != 0) {
        return;
    }
    if (*(signed char *)(p + 0x44) <= 0) {
        return;
    }
    if (*(unsigned short *)(p + 0xec) == 0) {
        return;
    }
    if (p[0x7e] != 0) {
        count = 1;
        h = 0xcc;
        if ((unsigned char)(p[0x7e] + 0xfbU) <= 2) {
            count = 3;
        }
        for (i = 0; i < count; i++) {
            ((Self02060ae0 *)p)->slot41();
            value = func_02022ca4(func_0200eb78(), *(unsigned short *)(p + 0x104));
            func_02060cf4(2);
            func_02060d00(value);
            if ((unsigned char)(p[0x7e] + 0xfbU) <= 2) {
                h = (i + 1) * 0x22;
                count = 3;
            }
            negh = (short)-h;
            right = 0x40000;
            for (j = 0; j < p[0x7f]; j++) {
                pts = *(struct Point02060ae0 **)(p + 0xe4);
                x = pts[j].x;
                y = pts[j].y;
                if (i > 0 && j != 0 && j < p[0x7f] - 1) {
                    x = x + data_0209dbd8[func_01ff80b8(func_02008eb0()) & 0xf];
                    y = y + data_0209dbd8[func_01ff80b8(func_02008eb0()) & 0xf];
                }
                data_020af554[15] = x >> 7;
                data_020af554[16] = (y / 3) >> 5;
                data_020af554[17] = -(func_02029708(func_0202970c()) - 0x8e0);
                func_0206f77c(data_020af56c);
                func_0206f7b4(*(unsigned char **)(p + 0xe8) + j * 0x24);
                if (j == 0) {
                    func_02060d0c(0, 0);
                    func_02060d2c(-0x80, h, 0);
                    func_02060d0c(0, 0x10000);
                    func_02060d2c(-0x80, negh, 0);
                }
                s = data_0209db98[j] << 12;
                func_02060d0c(s, 0);
                func_02060d2c(0, h, 0);
                func_02060d0c(s, 0x10000);
                func_02060d2c(0, negh, 0);
                if (j == p[0x7f] - 1) {
                    func_02060d0c(right, 0);
                    func_02060d2c(0x80, h, 0);
                    func_02060d0c(right, 0x10000);
                    func_02060d2c(0x80, negh, 0);
                }
            }
            func_02060d48();
        }
    }
    c = p[0x7e];
    if (c != 5 && c != 6 && c != 7) {
        func_0205f080(*(int *)(p + 0xdc));
    }
}
}
