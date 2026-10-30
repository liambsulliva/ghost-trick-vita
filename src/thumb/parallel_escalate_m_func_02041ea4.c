struct Flags020c9e48 {
    char pad[0x54];
    unsigned int flags;
};

struct Task02041ea4 {
    int unk0;
    unsigned char state;
};

extern struct Flags020c9e48 data_020c9e48;
extern char data_020cf9fc;
unsigned int func_02009900(void);
void func_0202fad4(unsigned int context, void *data);
void func_02022f6c(struct Task02041ea4 *object);

void func_02041ea4(struct Task02041ea4 *object)
{
    switch (object->state) {
    case 0:
        object->state++;
        return;
    case 1:
        data_020c9e48.flags &= ~0x1000;
        func_0202fad4(func_02009900(), &data_020cf9fc);
        func_02022f6c(object);
        return;
    }
}
