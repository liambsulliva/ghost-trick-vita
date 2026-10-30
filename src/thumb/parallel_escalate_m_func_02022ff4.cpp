class Scene02022ff4 {
public:
    virtual void slot0();
    virtual void slot1();
    virtual void slot2();
    virtual void slot3();
    virtual void slot4();
    virtual int slot5();
    virtual void slot6();
    virtual void slot7();
    int pad4;
    int pad8;
    unsigned char started;
    unsigned char padd;
    unsigned char done;
    unsigned char padf;
    unsigned char pad10;
    unsigned char blocking;
};

struct Stack02022ff4 {
    int pad0;
    Scene02022ff4 *scenes[8];
    int count;
};

extern "C" {
extern void func_02023094(Stack02022ff4 *stack);
extern void func_02023204(Stack02022ff4 *stack, Scene02022ff4 **slot);

void func_02022ff4(Stack02022ff4 *stack)
{
    Scene02022ff4 **p;
    Scene02022ff4 **q;

    func_02023094(stack);
    p = stack->scenes;
    do {
        if (*p == 0) {
            break;
        }
        if ((*p)->started == 0) {
            if ((*p)->slot5() == 0) {
                return;
            }
            (*p)->slot6();
            (*p)->started = 1;
        }
        if ((*p)->done == 0) {
            if ((*p)->blocking != 0) {
                break;
            }
            (*p)->slot7();
        }
        if ((*p)->done != 0) {
            q = &stack->scenes[7];
            do {
                if (*q != 0) {
                    func_02023204(stack, q);
                    stack->count--;
                }
                q--;
            } while (*q != *p);
            func_02023204(stack, q);
            stack->count--;
            return;
        }
        p++;
    } while (p < &stack->scenes[8]);
    func_02023094(stack);
}
}
