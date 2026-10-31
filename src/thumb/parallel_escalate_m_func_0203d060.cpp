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
    virtual void slot8();
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
    virtual void slot30();
    virtual void slot31();
    virtual void slot32();
    virtual void slot33();
    virtual void slot34();
    virtual void slot35(signed char value);
    virtual void slot36();
    virtual void slot37();
    virtual void slot38();
    virtual void slot39();
    virtual void slot40();
    virtual void slot41();
    virtual void slot42(unsigned int id, int a, int b, int c, int d, int e, int f);
};
extern "C" {
extern void func_0202f64c(unsigned char *slot);
int func_0203d060(unsigned char *p, int open, int flag, unsigned int id, int mode)
{
    int n;
    if (*(Object **)(p + 0x34) == 0) {
        return 1;
    }
    if (open == 0) {
        *(short *)(p + 0x44) += 8;
        if (*(short *)(p + 0x44) > 0x100) {
            *(short *)(p + 0x44) = 0x100;
            func_0202f64c(p + 0x30);
            *(Object **)(p + 0x30) = *(Object **)(p + 0x34);
            *(Object **)(p + 0x34) = 0;
            if (flag != 0 && *(Object **)(p + 0x2c) != 0) {
                (*(Object **)(p + 0x2c))->slot35(0x1f);
            }
            (*(Object **)(p + 0x30))->slot14(0);
            return 1;
        }
        if (*(short *)(p + 0x44) < 0x80) {
            n = (0x80 - *(short *)(p + 0x44)) >> 2;
            if (n > 0x1f) {
                n = 0x1f;
            }
            if (flag != 0 && *(Object **)(p + 0x2c) != 0) {
                (*(Object **)(p + 0x2c))->slot35(n);
            }
        } else {
            if (flag != 0) {
                if (*(short *)(p + 0x44) == 0x80 && *(Object **)(p + 0x2c) != 0) {
                    (*(Object **)(p + 0x2c))->slot42(id, 0, 1, 0, 1, -1, 0);
                }
                n = (*(short *)(p + 0x44) - 0x80) >> 2;
                if (n > 0x1f) {
                    n = 0x1f;
                }
                if (*(Object **)(p + 0x2c) != 0) {
                    (*(Object **)(p + 0x2c))->slot35(n);
                }
            }
            if (mode == 1) {
                n = (0x100 - *(short *)(p + 0x44)) >> 2;
                if (n > 0x1f) {
                    n = 0x1f;
                }
                if (*(Object **)(p + 0x38) != 0) {
                    (*(Object **)(p + 0x38))->slot35(n);
                }
                if (*(Object **)(p + 0x3c) != 0) {
                    (*(Object **)(p + 0x3c))->slot35(n);
                }
            }
        }
        if (*(Object **)(p + 0x30) != 0) {
            (*(Object **)(p + 0x30))->slot14(-*(short *)(p + 0x44) * 0x1000);
        }
        if (*(Object **)(p + 0x34) != 0) {
            (*(Object **)(p + 0x34))->slot14((0x100 - *(short *)(p + 0x44)) * 0x1000);
        }
        if (mode == 3) {
            if (*(Object **)(p + 0x38) != 0) {
                (*(Object **)(p + 0x38))->slot14((-0x2c - *(short *)(p + 0x44)) * 0x1000);
            }
            if (*(Object **)(p + 0x3c) != 0) {
                (*(Object **)(p + 0x3c))->slot14((0x2c - *(short *)(p + 0x44)) * 0x1000);
            }
        }
        if (mode == 2) {
            if (*(Object **)(p + 0x38) != 0) {
                (*(Object **)(p + 0x38))->slot14((0xd4 - *(short *)(p + 0x44)) * 0x1000);
            }
            if (*(Object **)(p + 0x3c) != 0) {
                (*(Object **)(p + 0x3c))->slot14((300 - *(short *)(p + 0x44)) * 0x1000);
            }
        }
    } else {
        *(short *)(p + 0x44) -= 8;
        if (*(short *)(p + 0x44) < 0) {
            *(short *)(p + 0x44) = 0;
            func_0202f64c(p + 0x30);
            *(Object **)(p + 0x30) = *(Object **)(p + 0x34);
            *(Object **)(p + 0x34) = 0;
            if (flag != 0 && *(Object **)(p + 0x2c) != 0) {
                (*(Object **)(p + 0x2c))->slot35(0x1f);
            }
            (*(Object **)(p + 0x30))->slot14(0);
            return 1;
        }
        if (*(short *)(p + 0x44) > 0x80) {
            n = (*(short *)(p + 0x44) - 0x80) >> 2;
            if (n > 0x1f) {
                n = 0x1f;
            }
            if (flag != 0 && *(Object **)(p + 0x2c) != 0) {
                (*(Object **)(p + 0x2c))->slot35(n);
            }
        } else {
            if (flag != 0) {
                if (*(short *)(p + 0x44) == 0x80 && *(Object **)(p + 0x2c) != 0) {
                    (*(Object **)(p + 0x2c))->slot42(id, 0, 1, 0, 1, -1, 0);
                }
                n = (0x80 - *(short *)(p + 0x44)) >> 2;
                if (n > 0x1f) {
                    n = 0x1f;
                }
                if (*(Object **)(p + 0x2c) != 0) {
                    (*(Object **)(p + 0x2c))->slot35(n);
                }
            }
            if (mode == 1) {
                n = *(short *)(p + 0x44) >> 2;
                if (n > 0x1f) {
                    n = 0x1f;
                }
                if (*(Object **)(p + 0x38) != 0) {
                    (*(Object **)(p + 0x38))->slot35(n);
                }
                if (*(Object **)(p + 0x3c) != 0) {
                    (*(Object **)(p + 0x3c))->slot35(n);
                }
            }
        }
        if (*(Object **)(p + 0x30) != 0) {
            (*(Object **)(p + 0x30))->slot14((0x100 - *(short *)(p + 0x44)) * 0x1000);
        }
        if (*(Object **)(p + 0x34) != 0) {
            (*(Object **)(p + 0x34))->slot14(-*(short *)(p + 0x44) * 0x1000);
        }
        if (mode == 3) {
            if (*(Object **)(p + 0x38) != 0) {
                (*(Object **)(p + 0x38))->slot14((0xd4 - *(short *)(p + 0x44)) * 0x1000);
            }
            if (*(Object **)(p + 0x3c) != 0) {
                (*(Object **)(p + 0x3c))->slot14((300 - *(short *)(p + 0x44)) * 0x1000);
            }
        }
        if (mode == 2) {
            if (*(Object **)(p + 0x38) != 0) {
                (*(Object **)(p + 0x38))->slot14((-0x2c - *(short *)(p + 0x44)) * 0x1000);
            }
            if (*(Object **)(p + 0x3c) != 0) {
                (*(Object **)(p + 0x3c))->slot14((0x2c - *(short *)(p + 0x44)) * 0x1000);
            }
        }
    }
    return 0;
}
}
