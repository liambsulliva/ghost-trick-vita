extern char data_020d0f0c;
typedef int code();
extern int func_02074074();
extern long long func_02075c98();

void func_0207403c(unsigned int argument)

{
    code *pcVar1;

    pcVar1 = *(code **)(**(int **)(((int)&data_020d0f0c) + 8) + 0xb4);
    if (pcVar1 != (code *)0x0) {
        *(unsigned int *)(**(int **)(((int)&data_020d0f0c) + 8) + 0xb4) = 0;
        (*pcVar1)(argument);
        func_02075c98();
    }
    func_02074074();
    return;
}
