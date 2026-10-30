extern void func_ov006_020d5ac0(unsigned char *, int);
extern void func_ov006_020d5d90(unsigned char *, int);
extern void func_ov006_020d5f94(unsigned char *, int);
extern void func_ov006_020d5fbc(unsigned char *, int);
extern void func_ov006_020d6028(unsigned char *, int);
extern void func_ov006_020d6050(unsigned char *, int);
extern void func_ov006_020d60a4(unsigned char *, int);
extern void func_ov006_020d6180(unsigned char *, int);
extern void func_ov006_020d624c(unsigned char *, int);
extern void func_ov006_020d628c(unsigned char *, int);

void func_ov006_020d5a28(unsigned char *param_1, int param_2)
{
    switch (param_1[0x26]) {
    case 0:
        func_ov006_020d5ac0(param_1, param_2);
        break;
    case 1:
        func_ov006_020d5d90(param_1, param_2);
        break;
    case 2:
        func_ov006_020d5f94(param_1, param_2);
        break;
    case 4:
    case 5:
        func_ov006_020d5fbc(param_1, param_2);
        break;
    case 6:
        func_ov006_020d6028(param_1, param_2);
        break;
    case 7:
        func_ov006_020d6050(param_1, param_2);
        break;
    case 8:
        func_ov006_020d60a4(param_1, param_2);
        break;
    case 9:
        func_ov006_020d6180(param_1, param_2);
        break;
    case 10:
        func_ov006_020d624c(param_1, param_2);
        break;
    case 11:
        func_ov006_020d628c(param_1, param_2);
        break;
    }
}
