extern int func_ov006_020d793c(unsigned char *, int, unsigned char, unsigned char);
extern void func_ov006_020d70f0(unsigned char *, int);
extern void func_0202f64c(unsigned char *);
struct State {
    unsigned int before_flags[21];
    unsigned int flags;
};

extern struct State data_020c9e48;

void func_ov006_020d5fbc(unsigned char *param_1, int param_2)
{
    if (func_ov006_020d793c(param_1, 0, param_1[0x25] & 1, param_1[0x25] >> 1) != 0) {
        if (param_1[0x26] == 4) {
            param_1[0x25] = param_1[0x25] + 2;
            func_ov006_020d70f0(param_1, param_2);
        } else {
            data_020c9e48.flags = data_020c9e48.flags & 0xfffffdff;
            func_0202f64c(param_1 + 0x434);
        }
        param_1[0x26] = 6;
    }
}
