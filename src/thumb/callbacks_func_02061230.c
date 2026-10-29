extern void func_02060d54();
typedef int code();

void func_02061230(int param_1)

{
    code *UNRECOVERED_JUMPTABLE;

    UNRECOVERED_JUMPTABLE = ((code *)((unsigned int)func_02060d54));
    *(unsigned char *)(param_1 + 0x7c) = *(unsigned char *)(param_1 + 0x7c) | 8;


    (*UNRECOVERED_JUMPTABLE)(param_1,0);
    return;
}
