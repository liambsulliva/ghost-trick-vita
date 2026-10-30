typedef void code();
extern unsigned int data_020c9e48;
extern void func_0203018c();

void func_02041ce8(void)
{
    code *UNRECOVERED_JUMPTABLE;

    data_020c9e48 = 0xfffffdffu & data_020c9e48;
    UNRECOVERED_JUMPTABLE = func_0203018c;
    (*UNRECOVERED_JUMPTABLE)();
    return;
}
