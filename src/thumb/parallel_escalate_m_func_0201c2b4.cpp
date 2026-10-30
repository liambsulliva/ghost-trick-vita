class Object0201c2b4 {
public:
    virtual void slot0();
    virtual void slot1();
    virtual void slot2();
    virtual void slot3();
    virtual void slot4();
    virtual void slot5();
    virtual void slot6();
    virtual void slot7();
    virtual void slot8(int);
    virtual void slot9();
    virtual void slot10();
    virtual void slot11();
    virtual void slot12();
    virtual void slot13();
};

extern "C" {
extern int func_0201c2f0(int *);
extern void func_0201c328(int *);

void func_0201c2b4(int *param_1)
{
    if (param_1[0xe] != 0) {
        if (param_1[0xe] == 1) {
            if (func_0201c2f0(param_1) != 0) {
                param_1[0xe] = 2;
            }
            else {
                func_0201c328(param_1);
            }
        }
        if (param_1[0xe] == 2) {
            ((Object0201c2b4 *)param_1)->slot13();
            ((Object0201c2b4 *)param_1)->slot8(0);
        }
    }
}
}
