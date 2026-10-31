class Object {
public:
    virtual void slot0();
    virtual void slot1();
    virtual void slot2();
    virtual void slot3();
    virtual void slot4();
    virtual void slot5();
    virtual void slot6();
    virtual void slot7();
    virtual void slot8(int value);
    virtual void slot9();
    virtual void slot10();
    virtual void slot11();
    virtual void slot12();
    virtual void slot13();
    virtual void slot14(int value);
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
    virtual void slot30(int value);
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
    virtual void slot42();
    virtual void slot43();
    virtual void slot44();
    virtual void slot45();
    virtual void slot46();
    virtual int slot47(int a, int b);
};
extern "C" {
extern void *func_02008444(void);
extern unsigned int func_02018db4(void *object);
extern void func_0203a338(unsigned char *p);
extern void *func_02010b7c(void);
extern unsigned int func_02015188(void *object);
extern int func_0202cb64(void *object);
extern int func_02015154(void *object);
extern int func_02010b60(void *object);
extern int func_02010b64(void *object);
extern unsigned int func_02037aa4(unsigned int kind);
extern void func_0203aa80(unsigned char *p);
extern void func_0203aab0(unsigned char *p);
extern void func_0203aaec(unsigned char *p);
extern void func_0203ad6c(unsigned char *p);
extern void func_0203ae1c(unsigned char *p);
extern int func_0203abc8(unsigned char *p, int value, int target);
extern unsigned int data_020cdd0c[];
void func_0203a73c(unsigned char *p)
{
    unsigned char state;
    int a;
    int b;
    if (func_02018db4(func_02008444()) != 0) {
        return;
    }
    state = p[0x10];
    if (state == 1) {
        if (p[0xf1] == 2) {
            p[0x10] = 2;
            p[0xf1] = 0;
        }
        return;
    }
    if (p[0xf0] != 0) {
        p[0xf0]--;
        return;
    }
    if (state == 0) {
        func_0203a338(p);
        *(int *)(p + 0x28) += 0xfa000;
        if (p[0xf1] == 2) {
            p[0x10] = 2;
            p[0xf1] = 0;
            *(int *)(p + 0xe8) = 0xc000;
        } else {
            p[0x10] = 1;
            *(int *)(p + 0xe8) = 0x8000;
        }
        (*(Object **)(p + 0xf8))->slot14(*(int *)(p + 0x28));
        return;
    }
    if (state == 5) {
        ((Object *)p)->slot8(0);
    }
    if (p[0x10] == 2) {
        func_0203aa80(p);
    }
    if (p[0x10] == 3) {
        func_0203aab0(p);
    }
    if (p[0x10] == 6) {
        if (func_02015188(func_02010b7c()) != 0) {
            p[0xe2] = 1;
            if (func_0202cb64(func_02010b7c()) != 0) {
                return;
            }
        }
        if (p[0xe2] != 0 && func_02015154(func_02010b7c()) != 0 && func_0202cb64(func_02010b7c()) == 0) {
            *(unsigned short *)(p + 0xd4) = func_02010b60(func_02010b7c());
            *(unsigned short *)(p + 0xd6) = func_02010b64(func_02010b7c());
            if (((Object *)p)->slot47(*(unsigned short *)(p + 0xd4), *(unsigned short *)(p + 0xd6)) == 0) {
                *(unsigned short *)(p + 0xd4) = 0;
                *(unsigned short *)(p + 0xd6) = 0;
                return;
            }
            p[0xdc] = 0;
            p[0xe0] = 0;
            p[0xe3] = 0;
            p[0xe4] = 0;
            p[0xe5] = 0;
            p[0x10] = 9;
        }
        if (p[0xe4] != 0) {
            func_0203ad6c(p);
            func_02037aa4(10);
            return;
        }
    }
    if (p[0xe3] == 0 && p[0xe2] != 0 && func_02015154(func_02010b7c()) == 0) {
        p[0xe3] = 1;
    }
    if (p[0x10] == 9) {
        if (func_02015154(func_02010b7c()) != 0) {
            if (data_020cdd0c[1]++ > 8) {
                data_020cdd0c[1] = 0;
                func_02037aa4(8);
            }
        }
        if (p[0xe3] != 0 || func_02015154(func_02010b7c()) == 0) {
            *(unsigned short *)(p + 0xd4) = 0;
            *(unsigned short *)(p + 0xd6) = 0;
            if (*(signed char *)(p + 0xdc) != 0) {
                func_0203ad6c(p);
            } else {
                p[0x10] = 6;
            }
            p[0xe2] = 0;
            p[0xe3] = 0;
            return;
        }
        if (p[0xe2] == 0) {
            return;
        }
        a = func_02010b60(func_02010b7c());
        b = func_02010b64(func_02010b7c());
        if (((Object *)p)->slot47(a, b) != 0) {
            if (p[0xe4] != 0) {
                func_0203ad6c(p);
                return;
            }
            if (func_0203abc8(p, a, b) != 0) {
                data_020cdd0c[1] = 0;
            }
        } else {
            func_0203ad6c(p);
            return;
        }
    }
    if (p[0x10] == 10) {
        if (*(signed char *)(p + 0xe0) > 0) {
            *(unsigned short *)(p + 0xda) += 0x555;
            (*(signed char *)(p + 0xe0))--;
        } else if (*(signed char *)(p + 0xe0) < 0) {
            *(unsigned short *)(p + 0xda) -= 0x555;
            (*(signed char *)(p + 0xe0))++;
        }
        ((Object *)p)->slot30(*(unsigned short *)(p + 0xda));
        (*(Object **)(p + 0xf8))->slot30(*(unsigned short *)(p + 0xda));
        if (*(signed char *)(p + 0xe0) == 0) {
            func_0203ae1c(p);
            p[0xe4] = 0;
            p[0x10] = 6;
        }
        p[0xf0] = 2;
        return;
    }
    if (p[0x10] == 7) {
        func_0203aaec(p);
    }
}
}
