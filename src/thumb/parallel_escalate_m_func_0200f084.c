extern char data_020cfb44;
extern char data_020cec8c;
extern char data_020cfb28;
extern void *func_02009900(void);
extern void *func_0205c030(void *param_1);
extern void *func_0205c044(void *param_1);
extern void *func_0204e2b4(void *param_1);
extern void func_0200f168(void *object, int value);
extern void func_0200f170(void *object, unsigned char value);
extern void func_0200a200(void *object);
extern void func_0200a260(void *object);
extern void func_0202f64c(void *object);
extern void func_0202f7e8(void *object, void *value, int param_3, int param_4);
extern void *func_02008444(void);
extern void func_020117a8(void *object, int value);

struct Flags8 {
    unsigned int low : 15;
    unsigned int active : 1;
    unsigned int high : 16;
};

struct FlagsC {
    unsigned int low : 12;
    unsigned int locked : 1;
    unsigned int high : 19;
};

void func_0200f084(char *param_1, unsigned int param_2, int param_3, int param_4, unsigned char param_5)
{
    void *object;

    if (*(void **)(param_1 + 0xee0) == 0) {
        return;
    }
    if (((struct Flags8 *)(param_1 + 8))->active == param_2) {
        return;
    }
    ((struct Flags8 *)(param_1 + 8))->active = (param_2 != 0);
    if (param_2 != 0) {
        if (param_3 != 0) {
            object = func_02009900();
            func_0202f7e8(object, func_0205c044(&data_020cfb44), 1, 0);
        }
        func_0200a200(*(void **)(param_1 + 0xee0));
        object = func_0204e2b4(&data_020cec8c);
        *(void **)(param_1 + 0xee8) = object;
        if (object != 0) {
            func_0202f7e8(func_02009900(), object, 1, 0);
        }
    } else {
        if (param_3 != 0) {
            object = func_0205c030(&data_020cfb28);
            if (object != 0) {
                func_0200f168(object, param_4);
                func_0200f170(object, param_5);
                func_0202f7e8(func_02009900(), object, 0xd, 0);
            }
        }
        func_0200a260(*(void **)(param_1 + 0xee0));
        func_0202f64c(param_1 + 0xee8);
        if (((struct FlagsC *)(param_1 + 0xc))->locked == 0 && param_3 != 0) {
            func_020117a8(func_02008444(), 1);
        }
    }
}
