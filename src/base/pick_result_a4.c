void func_02009750(void *object);
int func_020090d0(int c, int a, int b);
void func_02009760(void *object);

int func_020090a4(void *object, int a, int b, int c)
{
    int result;
    int code;
    func_02009750(object);
    result = func_020090d0(c, a, b);
    func_02009760(object);
    code = 1;
    if (result == 0) {
        code = 2;
    }
    return code;
}
