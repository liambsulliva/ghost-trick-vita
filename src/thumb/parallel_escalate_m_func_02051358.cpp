class Object;

extern "C" {
extern char data_020abc74;
extern char data_020abc84;
extern unsigned int *func_02008444(void);
extern int func_02027ea4(unsigned int *system);
extern int func_02023fdc(void *queue);
extern int func_0200d84c(Object *object, void *data);
extern void func_02051508(void *self, int flag);
extern void func_02050ea4(void *list, Object *object);
extern void func_020514d8(void *self);
extern void func_02050f24(void *list);
extern int func_02051448(void *queue);
}

class Holder {
public:
    virtual void s0();
    virtual void s1();
    virtual void s2();
    virtual void s3();
    virtual void s4();
    virtual void s5();
    virtual void s6();
    virtual void s7();
    virtual void s8(int value);

    char pad4[0x24];
    char queue[0x38];
    char list[0x54];
    void *main;
    Object *kids[16];
    unsigned int count;
    int padfc;
    unsigned char flag;
};

extern "C" void func_02051358(Holder *self)
{
    unsigned int i;

    if (func_02027ea4(func_02008444()) != 0) {
        return;
    }
    if (self->flag == 0) {
        while (func_02023fdc(self->queue) != 0) {
        }
        for (i = 0; i < self->count; i++) {
            if (self->kids[i] != 0 && func_0200d84c(self->kids[i], &data_020abc84) != 0) {
                func_02051508(self, 0);
            } else if (self->kids[i] != 0 && func_0200d84c(self->kids[i], &data_020abc74) != 0) {
                func_02051508(self, 1);
            } else {
                func_02050ea4(self->list, self->kids[i]);
            }
        }
        func_020514d8(self);
        if (self->flag == 0) {
            func_02050f24(self->list);
        }
    } else {
        for (i = 0; i < self->count; i++) {
            if (self->kids[i] != 0 && func_0200d84c(self->kids[i], &data_020abc84) != 0) {
                func_02051508(self, 0);
                break;
            }
        }
    }
    if (func_02051448(self->queue) != 0) {
        self->s8(0);
    }
}
