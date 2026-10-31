struct Watcher {
    char pad[0x38];
    void *child;
    int current;
    int target;
};

extern char *func_02008444(void);
extern int func_02010194(char *object);
extern unsigned int func_02011320(char *object);
extern int func_02058ab8(int target);
extern void func_020406e4(struct Watcher *watcher, int value, int flag);
extern void func_0204117c(void *child);

void func_02040570(struct Watcher *watcher)
{
    int target;

    target = func_02010194(func_02008444());
    if (target != 0) {
        if (watcher->target != target) {
            watcher->target = target;
            if (func_02011320(func_02008444()) == 4 || func_02011320(func_02008444()) == 0x16) {
                func_020406e4(watcher, func_02058ab8(target), 1);
            } else {
                func_020406e4(watcher, func_02058ab8(target), 0);
            }
        } else if (watcher->current != func_02058ab8(target)) {
            func_020406e4(watcher, func_02058ab8(target), 0);
        }
    }
    if (watcher->child != 0) {
        func_0204117c(watcher->child);
    }
}
