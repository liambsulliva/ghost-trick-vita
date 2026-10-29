extern char data_020d1180;
extern int func_02074ad4();
extern int func_02074bb4();
extern int func_02074c58();
extern int func_02074c6c();

void func_02074948(void)

{
    unsigned int uVar1;

    if (*((int *)&data_020d1180) == 0) {
        *((int *)&data_020d1180) = 1;
        uVar1 = func_02074ad4(0);
        func_02074c58(0,uVar1);
        uVar1 = func_02074bb4(0);
        func_02074c6c(0,uVar1);
        func_02074c6c(2,0);
        func_02074c58(2,0);
        uVar1 = func_02074ad4(3);
        func_02074c58(3,uVar1);
        uVar1 = func_02074bb4(3);
        func_02074c6c(3,uVar1);
        uVar1 = func_02074ad4(4);
        func_02074c58(4,uVar1);
        uVar1 = func_02074bb4(4);
        func_02074c6c(4,uVar1);
        uVar1 = func_02074ad4(5);
        func_02074c58(5,uVar1);
        uVar1 = func_02074bb4(5);
        func_02074c6c(5,uVar1);
        uVar1 = func_02074ad4(6);
        func_02074c58(6,uVar1);
        uVar1 = func_02074bb4(6);
        func_02074c6c(6,uVar1);
        return;
    }
    return;
}
