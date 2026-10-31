extern char data_020af5b0[];
extern void func_02056b2c(void *object);

struct Object {
    void *vtable;
    unsigned char sub[0x70d];
    unsigned char b711;
    signed char b712;
    signed char b713;
    short h714;
    short h716;
    short h718;
    short h71a;
    short h71c;
    short h71e;
    int w720;
};

struct Object *func_0205e8bc(struct Object *param_1)
{
    param_1->vtable = data_020af5b0;
    func_02056b2c(param_1->sub);
    param_1->b711 = 1;
    param_1->b712 = 0x1f;
    param_1->b713 = -1;
    param_1->h714 = 0;
    param_1->h716 = 0;
    param_1->h718 = 0;
    param_1->h71a = 0;
    param_1->h71c = 0;
    param_1->h71e = 0;
    param_1->w720 = 0;
    return param_1;
}
