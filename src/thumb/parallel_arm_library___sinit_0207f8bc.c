extern char data_020ce658[0x54];
extern char data_0209b4c8;
extern char data_020bbc18;
extern char data_020ce63c[0x18];
extern char data_0209b4d8;
extern char data_020c9fa4;
extern char data_020ce620;
void func_02042d58(void *a, void *b, void *c, int d, int e, int f);
void func_02042d74(void *a, void *b, void *c, int d, int e, int f);
void func_0200eaec(void *a);

void __sinit_0207f8bc(void)
{
    func_02042d58(&data_020ce658, &data_0209b4c8, &data_020bbc18, sizeof(data_020ce658), 0, 0);
    func_02042d74(&data_020ce63c, &data_0209b4d8, &data_020c9fa4, sizeof(data_020ce63c), 0, 0);
    func_0200eaec(&data_020ce620);
}
