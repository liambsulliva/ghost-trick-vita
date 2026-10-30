extern void *func_02010b7c(void);
extern unsigned int func_02015188(void *value);
extern int func_0202cb64(void *value);
extern int func_02010b60(void *value);
extern int func_02010b64(void *value);
extern long long func_02057718(int object, int first, int second);

int func_0204f39c(int param_1)
{
    void *value;
    unsigned int flag;
    int first;
    int second;
    long long result;

    value = func_02010b7c();
    flag = func_02015188(value);
    if (flag == 0) {
        return 0;
    }
    value = func_02010b7c();
    first = func_0202cb64(value);
    if (first != 0) {
        return 0;
    }
    value = func_02010b7c();
    first = func_02010b60(value);
    value = func_02010b7c();
    second = func_02010b64(value);
    result = func_02057718(param_1, first, second);
    return (int)result;
}
