extern unsigned int func_02075c98(void);
extern void func_02075cac(unsigned int bits);

typedef struct {
    char pad[0x28];
    unsigned int slot;
} Statics;

extern Statics data_020d0f0c;

unsigned int func_02074360(unsigned int param_1)
{
    unsigned int saved;
    unsigned int old;

    saved = func_02075c98();
    old = data_020d0f0c.slot;
    data_020d0f0c.slot = param_1;
    func_02075cac(saved);
    return old;
}
