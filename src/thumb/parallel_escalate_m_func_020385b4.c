struct Self020385b4 {
    void *object;
    char pad04[0x18];
    int index;
    char pad20[6];
    unsigned short timer;
    char pad28[0xb];
    unsigned char active;
};

extern int func_02038220(struct Self020385b4 *self);
extern int func_0203838c(struct Self020385b4 *self, int index);
extern void func_02039dbc(void *object, int index, int mode);
extern void *func_02039ea0(void *object);
extern void func_02039264(void *item);
extern void func_020394c0(void *item);

int func_020385b4(struct Self020385b4 *self)
{
    void *item;
    unsigned short timer;

    if (self->active != 0) {
        if (func_02038220(self) != 0 && func_0203838c(self, self->index + 1) == 0) {
            func_02039dbc(self->object, self->index, 2);
            item = func_02039ea0(self->object);
            if (item != 0) {
                func_02039264(item);
                func_020394c0(item);
            }
            self->active = 0;
            self->timer = 0x52;
        }
    } else {
        timer = self->timer;
        if (timer != 0) {
            self->timer = timer - 1;
        } else {
            return 1;
        }
    }
    return 0;
}
