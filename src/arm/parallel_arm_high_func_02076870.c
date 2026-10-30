extern int data_020d1328;
extern int data_020d132c;
extern void func_02074564();
extern void func_020768d0();
extern void func_02077024();

void func_02076870(void)
{
    if (data_020d1328 == 0) {
        data_020d1328 = 1;
        func_02074564(&data_020d132c);
        func_020768d0();
        func_02077024();
    }
}
