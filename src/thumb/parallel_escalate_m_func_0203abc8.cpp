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
    virtual void slot30(int value);
    virtual int slot31();
};
extern "C" {
extern int func_02037aa4(int kind);
extern void func_020386f4(void *object, signed char value);
extern int func_02038400(void *object, int dir);
extern unsigned int func_0203ae58(unsigned char *p, int dir);
int func_0203abc8(unsigned char *p, unsigned short value, unsigned int target)
{
    int dir;
    int diff;
    unsigned int cur;
    dir = 0;
    diff = 0;
    if (target > *(unsigned short *)(p + 0xd6)) {
        dir = -1;
    } else if (*(unsigned short *)(p + 0xd6) > target) {
        dir = 1;
    }
    if (p[0xde] == 0 && dir != 0) {
        p[0xdc] = dir;
    }
    cur = *(unsigned short *)(p + 0xd6);
    if (target > cur) {
        diff = (unsigned short)(target - cur);
    } else if (cur > target) {
        diff = (unsigned short)(cur - target);
    }
    if (diff > 5) {
        p[0xde]++;
    }
    if (p[0xde] != 0) {
        if (func_02038400(*(void **)(p + 0xf4), *(signed char *)(p + 0xdc)) != 0) {
            if (p[0xe4] == 0 && p[0xe5] < 1u) {
                if (func_0203ae58(p, dir) == 0) {
                    return 0;
                }
            }
            *(unsigned short *)(p + 0xd4) = value;
            *(unsigned short *)(p + 0xd6) = target;
            return 1;
        }
        if (func_02037aa4(6) == 0) {
            return 0;
        }
        func_02037aa4(9);
        p[0xe4] = 0;
        func_020386f4(*(void **)(p + 0xf4), -*(signed char *)(p + 0xdc));
        *(unsigned short *)(p + 0xda) += (unsigned short)(-*(signed char *)(p + 0xdc) * 0x555);
        ((Object *)p)->slot30(*(unsigned short *)(p + 0xda));
        (*(Object **)(p + 0xf8))->slot30(*(unsigned short *)(p + 0xda));
        *(unsigned short *)(p + 0xd4) = value;
        p[0xe1]++;
        if (p[0xe1] > 3) {
            p[0xde] = 0;
            p[0xe1] = 0;
            if (diff > 10) {
                *(unsigned short *)(p + 0xd6) -= (unsigned short)(diff / (diff / 15) * *(signed char *)(p + 0xdc));
            } else {
                *(unsigned short *)(p + 0xd6) = target;
            }
            *(unsigned short *)(p + 0xda) = ((Object *)p)->slot31();
            ((Object *)p)->slot30(0);
        }
        p[0xf0] = 1;
        return 1;
    }
    return 0;
}
}
