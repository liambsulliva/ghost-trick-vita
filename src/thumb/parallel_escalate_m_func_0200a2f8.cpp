class Object0200a2f8 {
public:
    virtual void Slot0();
    virtual void Release();
};

extern "C" {

void *func_02031254(unsigned int size);
void *func_0200a378(void *object);
int func_02009fd8(char *entry);
void func_0200a368(char *entry, int value);
int func_02009df0(char *entry);
int func_0200a780(char *entry);
void func_0200d114(void *object, int a, int b, int c, int d);

void func_0200a2f8(char *param_1, int param_2)
{
    unsigned int i;
    char *obj6;
    char *entry;
    Object0200a2f8 *self;
    int value;
    int shift;

    obj6 = (char *)func_02031254(4);
    if (obj6 != 0) {
        obj6 = (char *)func_0200a378(obj6);
    }
    if (obj6 == 0) {
        return;
    }
    shift = param_2 >> 4;
    for (i = 0; i < 16; i = i + 1) {
        entry = param_1 + (i << 4);
        if (func_02009fd8(entry) == 0) {
            func_0200a368(entry, param_2);
            if (func_02009df0(entry) != 0) {
                value = func_0200a780(entry);
                if (value != 0) {
                    func_0200d114(obj6, value, value, 0x200, shift);
                }
            }
        }
    }
    if (obj6 != 0) {
        self = (Object0200a2f8 *)obj6;
        self->Release();
    }
}

}
