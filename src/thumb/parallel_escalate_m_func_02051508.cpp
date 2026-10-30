class Object {
public:
    virtual void slot0();
    virtual void slot1();
};

struct Holder {
    char pad0[0xb4];
    void *main;
    Object *kids[16];
    unsigned int count;
    int padfc;
    unsigned char flag;
};

extern "C" {
extern char data_020abc74;
extern char data_020abc84;
extern void func_0201001c(void *object);
extern void func_02051594(void *object);
extern int func_0200d84c(Object *object, void *data);
}

extern "C" void func_02051508(Holder *self, int flag)
{
    unsigned int i;
    self->flag = flag;
    if (self->main != 0) {
        if (flag != 0) {
            func_0201001c(self->main);
        } else {
            func_02051594(self->main);
        }
    }
    for (i = 0; i < self->count; i++) {
        if (self->kids[i] != 0) {
            if (flag == 0) {
                if (func_0200d84c(self->kids[i], &data_020abc74) != 0) {
                    if (self->kids[i] != 0) {
                        self->kids[i]->slot1();
                    }
                    self->kids[i] = 0;
                }
            } else {
                if (func_0200d84c(self->kids[i], &data_020abc84) != 0) {
                    if (self->kids[i] != 0) {
                        self->kids[i]->slot1();
                    }
                    self->kids[i] = 0;
                }
            }
        }
    }
}
