struct WithMethods
{
    void *methods;
};

extern char data_020ca22c;
extern char data_02085564;
extern char data_020bbc18;
extern char data_020ca248;
extern char data_02085570;
extern char data_020cf724;
extern char data_020ca264;
extern char data_02085588;
extern char data_020ca210;
extern char data_02085594;
extern char data_020ca280;
extern char data_020855a0;
extern char data_020ca29c;
extern char data_020855ac;
extern char data_020ca2b8;
struct WithMethods *func_0202e3c4(void *object, void *a, void *b, int c, int d, int e);
struct WithMethods *func_0202e3e0(void *object, void *a, void *b, int c, int d, int e);
struct WithMethods *func_0202e3fc(void *object, void *a, void *b, int c, int d, int e);
struct WithMethods *func_0202e418(void *object, void *a, void *b, int c, int d, int e);
struct WithMethods *func_0202e434(void *object, void *a, void *b, int c, int d, int e);
struct WithMethods *func_0202e450(void *object, void *a, void *b, int c, int d, int e);
extern int func_0207dd08();
extern void func_0202e46c(void);

void __sinit_0207ea60(void)
{
    char *puVar1;
    char *puVar2;

    puVar1 = &data_020ca22c;
    func_0202e3c4(puVar1, &data_02085564, &data_020bbc18, 132, 0, 0);
    func_0202e3e0(&data_020ca248, &data_02085570, &data_020cf724, 204, 0, 0);
    func_0202e3fc(&data_020ca264, &data_02085588, puVar1, 452, 0, 0);
    func_0202e418(&data_020ca210, &data_02085594, puVar1, 176, 0, 0);
    puVar2 = &data_020ca280;
    func_0202e434(puVar2, &data_020855a0, puVar1, 324, 0, 0);
    func_0202e450(&data_020ca29c, &data_020855ac, puVar2, 504, 0, 0);
    func_0207dd08(&data_020ca2b8, 4, 8, ((unsigned int)func_0202e46c), 0);
}
