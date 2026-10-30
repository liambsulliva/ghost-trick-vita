struct WithMethods {
    void *methods;
};

extern char data_020cf110[340];
extern char data_0209d4d0;
extern char data_020cf87c;
extern char data_020cf0f4[340];
extern char data_0209d4dc;
extern char data_020cf12c[340];
extern char data_0209d4ec;
extern char data_020cf20c[340];
extern char data_0209d500;
extern char data_020cf148[340];
extern char data_0209d514;
extern char data_020cf228[340];
extern char data_0209d528;
extern char data_020cf164[340];
extern char data_0209d538;
extern char data_020cf180[340];
extern char data_0209d54c;
extern char data_020cf19c[340];
extern char data_0209d558;
extern char data_020cf1b8[344];
extern char data_0209d564;
extern char data_020cf1d4[344];
extern char data_0209d57c;
extern char data_020cf1f0[344];
extern char data_0209d598;
struct WithMethods *func_02053414(void *object, void *a, void *b, int c, int d, int e);
struct WithMethods *func_02053430(void *object, void *a, void *b, int c, int d, int e);
struct WithMethods *func_0205344c(void *object, void *a, void *b, int c, int d, int e);
struct WithMethods *func_02053468(void *object, void *a, void *b, int c, int d, int e);
struct WithMethods *func_02053484(void *object, void *a, void *b, int c, int d, int e);
struct WithMethods *func_020534a0(void *object, void *a, void *b, int c, int d, int e);
struct WithMethods *func_020534bc(void *object, void *a, void *b, int c, int d, int e);
struct WithMethods *func_020534d8(void *object, void *a, void *b, int c, int d, int e);
struct WithMethods *func_020534f4(void *object, void *a, void *b, int c, int d, int e);
struct WithMethods *func_02053510(void *object, void *a, void *b, int c, int d, int e);
struct WithMethods *func_0205352c(void *object, void *a, void *b, int c, int d, int e);
struct WithMethods *func_02053548(void *object, void *a, void *b, int c, int d, int e);

void __sinit_02080138(void)
{
    unsigned char *first;
    unsigned char *second;
    unsigned char *third;
    first = (unsigned char *)&data_020cf110;
    func_02053414(first, &data_0209d4d0, &data_020cf87c, sizeof(data_020cf110), 0, 0);
    func_02053430(&data_020cf0f4, &data_0209d4dc, first, sizeof(data_020cf0f4), 0, 0);
    func_0205344c(&data_020cf12c, &data_0209d4ec, first, sizeof(data_020cf12c), 0, 0);
    func_02053468(&data_020cf20c, &data_0209d500, first, sizeof(data_020cf20c), 0, 0);
    func_02053484(&data_020cf148, &data_0209d514, first, sizeof(data_020cf148), 0, 0);
    second = (unsigned char *)&data_020cf228;
    func_020534a0(second, &data_0209d528, first, sizeof(data_020cf228), 0, 0);
    func_020534bc(&data_020cf164, &data_0209d538, second, sizeof(data_020cf164), 0, 0);
    func_020534d8(&data_020cf180, &data_0209d54c, first, sizeof(data_020cf180), 0, 0);
    func_020534f4(&data_020cf19c, &data_0209d558, first, sizeof(data_020cf19c), 0, 0);
    third = (unsigned char *)&data_020cf1b8;
    func_02053510(third, &data_0209d564, first, sizeof(data_020cf1b8), 0, 0);
    func_0205352c(&data_020cf1d4, &data_0209d57c, third, sizeof(data_020cf1d4), 0, 0);
    func_02053548(&data_020cf1f0, &data_0209d598, third, sizeof(data_020cf1f0), 0, 0);
}
