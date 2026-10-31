extern void func_0203a348();
typedef int code();

struct Obj {
    unsigned int pad0[44];
    unsigned int b0;
    unsigned int b4;
    unsigned int pad1[5];
    unsigned int cc;
    unsigned int d0;
    unsigned int d4;
};

void func_02039348(struct Obj *param_1)
{
    code *UNRECOVERED_JUMPTABLE;

    param_1->d0 = param_1->b0;
    UNRECOVERED_JUMPTABLE = ((code *)((unsigned int)func_0203a348));
    param_1->d4 = param_1->b4;
    (*UNRECOVERED_JUMPTABLE)(param_1->cc);
    return;
}
