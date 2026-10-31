class Gauge {
public:
    virtual void v00();
    virtual void v04();
    virtual void v08();
    virtual void v0c();
    virtual void v10();
    virtual void v14();
    virtual void v18();
    virtual void v1c();
    virtual void v20(int value);
    unsigned char pad004[0xb8];
    int value;
    int falling;
};
extern "C" {
extern void *func_02008444(void);
extern int func_0200f330(void *object);
extern void func_02048f9c(Gauge *object);

void func_02048ea0(Gauge *param_1)
{
    int value;

    if (func_0200f330(func_02008444()) == 0) {
        if (param_1->falling != 0) {
            value = param_1->value - 0x200;
            param_1->value = value;
            if (value < 0) {
                param_1->value = 0;
                param_1->v20(0);
                return;
            }
        } else {
            value = param_1->value + 0x200;
            param_1->value = value;
            if (value > 0x40000) {
                param_1->value = 0x40000;
            }
        }
        func_02048f9c(param_1);
    }
}
}
