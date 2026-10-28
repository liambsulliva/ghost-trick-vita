struct Command {
    unsigned short unknown_0;
    unsigned short a;
    unsigned short b;
    unsigned short c;
};

struct Owner {
    unsigned char before[0x14];
    void *object;
};

extern char data_020cf898;
void *func_020082f8(void *object, void *kind);
void func_02059170(void *object, int a, int b, int c);

void func_020084f0(struct Owner *owner, struct Command *command)
{
    void *object = func_020082f8(owner->object, &data_020cf898);
    if (object != 0) {
        func_02059170(object, command->a, command->b, command->c);
    }
}
