extern char data_020d02a0;
typedef int code();
extern int func_0206a948();
extern int func_0206a97c();
extern int func_0206b168();
extern int func_020735f0();
extern int func_02073620();

struct Job {
    int unk0;
    code *callback;
    unsigned int arg;
    unsigned int dst;
    unsigned int src;
    int remain;
};

struct Ctx {
    int unk0;
    int unk4;
    struct Job *job;
};

extern struct Ctx data_020d08e0;

void func_0206ac04(void)
{
    struct Job *job;
    int remain;

    job = data_020d08e0.job;
    if (job == 0) {
        return;
    }
    job->dst = job->dst + 0x200;
    job->src = job->src + 0x200;
    remain = job->remain - 0x200;
    job->remain = remain;
    if (remain != 0) {
        func_0206a948(job->dst);
        return;
    }
    (**(code **)(*(int *)(((int)&data_020d02a0) + 0x50c) + 4))(*(unsigned int *)(((int)&data_020d02a0) + 0x508));
    func_020735f0(0x80000);
    func_02073620(0x80000);
    data_020d08e0.job = 0;
    func_0206b168(func_0206a97c());
    if (job->callback == 0) {
        return;
    }
    job->callback(job->arg);
}
