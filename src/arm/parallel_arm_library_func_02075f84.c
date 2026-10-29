typedef int code();
struct State {
    code *callback;
    unsigned int argument;
};
extern struct State data_020d129c;
extern void func_02075fb4();

void func_02075f84(void)
{
    code *callback;

    callback = data_020d129c.callback;
    if (callback != (code *)0) {
        data_020d129c.callback = 0;
        (*callback)(data_020d129c.argument);
    }
    func_02075fb4();
}
