class Object02016aac_0 {
public:
    virtual ~Object02016aac_0();
};

extern "C" {
extern char data_020a36d0;
typedef int code(...);
extern int func_02016504(...);
extern int func_02031270(...);

unsigned int * func_02016aac(unsigned int *param_1)

{
    *param_1 = ((unsigned int)&data_020a36d0);
    if ((int *)param_1[3] != (int *)0x0) {
        delete ((Object02016aac_0 *)(param_1[3]));
        param_1[3] = 0;
    }
    func_02016504(param_1);
    func_02031270(param_1);
    return param_1;
}
}
