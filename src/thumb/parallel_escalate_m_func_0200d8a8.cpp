class Self {
public:
    virtual void slot0();
    virtual void slot1();
    virtual void slot2(int param_1, void *param_2);
};

extern "C" {
extern Self *data_020b8774;
extern char data_02083be4;
extern char data_02083bf8;
unsigned char *func_02031254(int size);
unsigned char *func_0201e2cc(unsigned char *memory);
int func_0201e2ec(unsigned char *object, int a, int b, int c, int d);
void func_0200c530(Self *object, void *name);

void func_0200d8a8(void)
{
    unsigned char *object;

    if (data_020b8774 == 0) {
        object = func_02031254(0x20);
        if (object != 0) {
            object = func_0201e2cc(object);
        }
        data_020b8774 = (Self *)object;
        func_0201e2ec(object, 0x10, 0x100, 0, 0);
        func_0200c530(data_020b8774, &data_02083be4);
    }
    data_020b8774->slot2(0, &data_02083bf8);
}
}
