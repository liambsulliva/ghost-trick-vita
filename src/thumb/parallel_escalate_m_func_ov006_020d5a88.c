extern void func_ov006_020d633c(unsigned char *param_1, int param_2);
extern void func_ov006_020d66bc(unsigned char *param_1, int param_2);
extern void func_ov006_020d6bb4(unsigned char *param_1, int param_2);
extern void func_ov006_020d6bf8(unsigned char *param_1);

void func_ov006_020d5a88(unsigned char *param_1, int param_2)
{
    switch (param_1[0x26]) {
    case 0:
        func_ov006_020d633c(param_1, param_2);
        break;
    case 1:
        func_ov006_020d66bc(param_1, param_2);
        break;
    case 4:
        func_ov006_020d6bb4(param_1, param_2);
        break;
    case 6:
        func_ov006_020d6bf8(param_1);
        break;
    }
}
