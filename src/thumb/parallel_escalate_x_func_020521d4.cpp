class Obj020521d4 {
public:
    virtual void s0();
    virtual void s1();
    virtual void s2();
    virtual void s3();
    virtual void s4();
    virtual void s5();
    virtual void s6();
    virtual void s7();
    virtual void s8();
    virtual void s9();
    virtual void s10();
    virtual void s11();
    virtual void s12(int a, unsigned int b, int c);
};

struct Mode020521d4 {
    int state;
    int value;
};

struct Clock020521d4 {
    int pad[2];
    int now;
};

struct Info020521d4 {
    char pad[0x12];
    short level;
};

extern "C" {
extern Mode020521d4 data_020abf34;
extern Clock020521d4 data_020cff60;
extern int data_020cefc0;
extern Info020521d4 data_020c9e88;
extern char data_020cf010;
unsigned int func_0202f684(int a);
int func_02008828(void *a, void *b);
int func_02009900(void);
void func_0202f7e8(int a, void *b, int c, int d);

void func_020521d4(int param_1, int param_2, unsigned int param_3, int param_4)
{
    Obj020521d4 *obj;
    int now;

    obj = (Obj020521d4 *)func_0202f684(param_1);
    if (obj == 0) {
        return;
    }
    if (param_2 == 0 && func_02008828(obj, &data_020cf010) != 0) {
        switch (data_020abf34.state) {
        case 0:
            if (data_020c9e88.level > 8 || param_3 >= 12) {
                break;
            }
            now = data_020cff60.now;
            if ((unsigned int)(now - data_020cefc0) <= 10) {
                param_4 = data_020abf34.value / 2;
                data_020abf34.value = param_4;
            } else {
                param_4 = 0xc0;
                data_020abf34.value = param_4;
            }
            data_020cefc0 = now;
            break;
        case 1:
            if (data_020c9e88.level > 8 || param_3 >= 12) {
                break;
            }
            now = data_020cff60.now;
            param_4 = 0x40;
            if ((unsigned int)(now - data_020cefc0) > 10) {
                param_4 = 0xc0;
            }
            data_020cefc0 = now;
            break;
        case 2:
            if (data_020c9e88.level > 8 || param_3 >= 12) {
                break;
            }
            if (data_020abf34.value != 0 && (unsigned int)(data_020cff60.now - data_020cefc0) <= 10) {
                param_4 = 0;
                data_020abf34.value = param_4;
            } else {
                param_4 = 0xc0;
                data_020abf34.value = param_4;
            }
            data_020cefc0 = data_020cff60.now;
            break;
        case 3:
            if (data_020c9e88.level > 8 || param_3 >= 12) {
                break;
            }
            param_4 = data_020abf34.value;
            data_020cefc0 = data_020cff60.now;
            break;
        }
    }
    obj->s12(param_2, param_3, param_4);
    func_0202f7e8(func_02009900(), obj, 0xd, 0);
}
}
