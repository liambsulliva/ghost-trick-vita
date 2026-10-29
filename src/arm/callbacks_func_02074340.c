extern void func_02074188();
typedef int code();

void func_02074340(int *param_1)

{
    code *UNRECOVERED_JUMPTABLE;
    int iVar1;

    iVar1 = *param_1;
    *param_1 = 0;
    UNRECOVERED_JUMPTABLE = ((code *)((unsigned int)func_02074188));
    *(unsigned int *)(iVar1 + 0xb0) = 0;


    (*UNRECOVERED_JUMPTABLE)(iVar1);
    return;
}
