extern char *func_02008444(void);
extern unsigned int func_02011320(char *object);
extern unsigned int func_0200bc10(int param_1);
extern void func_0200bb8c(int param_1, unsigned int param_2);

void func_02052fa0(int param_1)
{
    unsigned short id;

    if (func_02011320(func_02008444()) == 5 || func_02011320(func_02008444()) == 8) {
        id = func_0200bc10(param_1 + 0x28) ? 0x8b : 0x85;
    } else {
        id = func_0200bc10(param_1 + 0x28) ? 0x8a : 0x84;
    }
    func_0200bb8c(param_1 + 0x28, id);
}
