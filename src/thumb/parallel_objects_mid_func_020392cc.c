extern void func_0203a338();
typedef int code();

struct Obj {
    unsigned int pad0[44];
    unsigned int b0;
    unsigned int b4;
    unsigned int pad1[3];
    unsigned int c4;
    unsigned int c8;
    unsigned int cc;
};

void func_020392cc(struct Obj *param_1)
{
    code *UNRECOVERED_JUMPTABLE;

    param_1->c4 = param_1->b0;
    UNRECOVERED_JUMPTABLE = ((code *)((unsigned int)func_0203a338));
    param_1->c8 = param_1->b4;
    (*UNRECOVERED_JUMPTABLE)(param_1->cc);
    return;
}
