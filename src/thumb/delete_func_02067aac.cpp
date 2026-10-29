class Object02067aac_0 {
public:
    virtual ~Object02067aac_0();
};

extern "C" {
extern char data_020b163c;
typedef int code(...);
extern int func_0201003c(...);
extern int func_02015c8c(...);
extern int func_02056cac(...);

unsigned int * func_02067aac(unsigned int *param_1)

{
    *param_1 = ((unsigned int)&data_020b163c);
    if ((int *)param_1[0x33] != (int *)0x0) {
        delete ((Object02067aac_0 *)(param_1[0x33]));
        param_1[0x33] = 0;
    }
    func_0201003c();
    func_02015c8c();
    func_02056cac(param_1);
    return param_1;
}
}
