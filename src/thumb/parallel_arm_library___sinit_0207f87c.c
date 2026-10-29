extern char data_020ce5e0[0x24];
extern char data_0209b4bc;
extern char data_020c9fa4;
extern char data_020ce5fc;
extern char data_020ce5d4;
void func_020426d4(void *a, void *b, void *c, int d, int e, int f);
void func_02042294(void *a);
void func_0207da20(int a, int b, void *c);
void func_020422dc(void);

void __sinit_0207f87c(void)
{
    unsigned char *object;

    func_020426d4(&data_020ce5e0, &data_0209b4bc, &data_020c9fa4, sizeof(data_020ce5e0), 0, 0);
    object = (unsigned char *)&data_020ce5fc;
    func_02042294(object);
    func_0207da20((int)object, (int)((unsigned int)func_020422dc), &data_020ce5d4);
}
