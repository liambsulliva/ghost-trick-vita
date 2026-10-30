void func_ov006_020d5698(unsigned int *, unsigned int);
void func_ov006_020d5680();

void func_ov006_020d5640(unsigned int *param_1, unsigned int *param_2)
{
    unsigned int limit;
    unsigned int index;
    unsigned int base;

    limit = param_1[1];
    index = param_1[0];
    if (index < limit) {
        param_1[0] = index + 1;
        func_ov006_020d5680(param_1[2] + index * 12, param_2);
    }
    else {
        func_ov006_020d5698(param_1, (limit + 1) << 1);
        index = param_1[0];
        base = param_1[2];
        param_1[0] = index + 1;
        func_ov006_020d5680(base + index * 12, param_2);
    }
}
