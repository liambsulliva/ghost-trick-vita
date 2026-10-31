extern void func_02012574();
extern void func_0202fbcc();
extern int func_0200eac4();
extern unsigned char *func_0200eab4();
extern int func_02008828();
extern unsigned char *func_0200eab8();
extern int func_02051614();
extern void func_0200eabc();
extern char data_020cee88;

int func_02012518(void)
{
    unsigned char auStack_28[16];
    unsigned int auStack_38[4];

    func_02012574(auStack_28);
    func_0202fbcc(auStack_38, 5);
    while (func_0200eac4(auStack_38) != 0) {
        if (func_02008828(func_0200eab4(auStack_38), &data_020cee88) != 0) {
            if (func_02051614(func_0200eab8(auStack_38), auStack_28) != 0) {
                return 1;
            }
        }
        func_0200eabc(auStack_38, 0);
    }
    return 0;
}
