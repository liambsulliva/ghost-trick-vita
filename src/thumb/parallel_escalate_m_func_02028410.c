struct ScriptMachine;

extern unsigned char *func_02028148(struct ScriptMachine *machine);
extern int func_020283fc(unsigned char *object);
extern int func_02028400(unsigned char *object);
extern int func_02028404(unsigned char *object);
extern int func_02028408(unsigned char *object);
extern int func_0202840c(unsigned char *object);
extern void func_02028504(unsigned char *object, unsigned char value);
extern void func_02028508(unsigned char *object, unsigned char value);
extern void func_0202850c(unsigned char *object, unsigned char value);
extern void func_02028510(unsigned char *object, unsigned char value);
extern void func_02028514(unsigned char *object, unsigned char value);

void func_02028410(struct ScriptMachine *machine, unsigned int which, int value, int relative)
{
    unsigned char *object;

    object = func_02028148(machine);
    if (object == 0) {
        return;
    }
    switch (which) {
    case 0xc:
        if (relative == 0) {
            func_02028504(object, value);
        } else {
            func_02028504(object, value + func_020283fc(object));
        }
        break;
    case 0xd:
        if (relative == 0) {
            func_02028508(object, value);
        } else {
            func_02028508(object, value + func_02028400(object));
        }
        break;
    case 0xe:
        if (relative == 0) {
            func_0202850c(object, value);
        } else {
            func_0202850c(object, value + func_02028404(object));
        }
        break;
    case 0xf:
        if (relative == 0) {
            func_02028510(object, value);
        } else {
            func_02028510(object, value + func_02028408(object));
        }
        break;
    case 0x10:
        if (relative == 0) {
            func_02028514(object, value);
        } else {
            func_02028514(object, value + func_0202840c(object));
        }
        break;
    case 0x1e:
        if (relative == 0) {
            *(int *)((char *)machine + 0x18) = value;
        } else {
            *(int *)((char *)machine + 0x18) = *(int *)((char *)machine + 0x18) + value;
        }
        break;
    }
}
