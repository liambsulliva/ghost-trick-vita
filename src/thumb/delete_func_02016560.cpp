class Object02016560_0 {
public:
    virtual ~Object02016560_0();
};

class Object02016560_1 {
public:
    virtual ~Object02016560_1();
};

extern "C" {
extern char data_020a3784;
typedef int code(...);
extern int func_02016504(...);
extern int func_02031270(...);

unsigned int * func_02016560(unsigned int *param_1)

{
    *param_1 = ((unsigned int)&data_020a3784);
    if ((int *)param_1[3] != (int *)0x0) {
        delete ((Object02016560_0 *)(param_1[3]));
        param_1[3] = 0;
    }
    if ((int *)param_1[4] != (int *)0x0) {
        delete ((Object02016560_1 *)(param_1[4]));
        param_1[4] = 0;
    }
    func_02016504(param_1);
    func_02031270(param_1);
    return param_1;
}
}
