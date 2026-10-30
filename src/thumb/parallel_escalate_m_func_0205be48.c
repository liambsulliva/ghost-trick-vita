typedef struct {
    int first;
    int second;
} Pair;

extern char data_0209daa8;
extern char data_020a2938;
extern char data_020aed28;
extern char data_020aed30;
extern int func_0200b9b8();
extern int func_02008a00();
extern int func_020089fc();

void func_0205be48(int param_1, int param_2)
{
    unsigned char buf[56];
    Pair pos;

    pos = *(Pair *)&data_020a2938;
    func_0200b9b8(buf, param_1, 6, (int)&data_0209daa8, *(Pair *)&data_020aed28, *(Pair *)&data_020aed30, 0, pos, pos);
    func_02008a00(param_2, buf);
    func_020089fc(buf);
}
