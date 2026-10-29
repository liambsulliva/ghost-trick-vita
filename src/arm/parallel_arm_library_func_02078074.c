struct State {
    unsigned char before[44];
    unsigned int busy;
    void (*callback)(unsigned int,unsigned int,struct State *,unsigned int);
    unsigned int extra;
};
extern struct State data_020d2ec0;

void func_02078074(unsigned int arg)
{
    void (*callback)(unsigned int,unsigned int,struct State *,unsigned int);
    unsigned int extra;

    callback = data_020d2ec0.callback;
    extra = data_020d2ec0.extra;
    data_020d2ec0.busy = 0;
    if (callback == 0) {
        return;
    }
    data_020d2ec0.callback = 0;
    (*callback)(arg,extra,&data_020d2ec0,0);
}
