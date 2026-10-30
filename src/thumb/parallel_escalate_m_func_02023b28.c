struct SceneState {
    unsigned char pad0;
    unsigned char pad1;
    unsigned char next;
    unsigned char current;
    void *pending;
    void *active;
};

extern struct SceneState data_020c9e04;
extern char data_020c9e05[];
extern char data_020c9e2c[];
extern char data_020c9e48[];
extern char data_020c9e9c[];
extern char data_020c9ef0[];
extern char data_020c9f44[];
extern void func_02023b9c(int mode);
extern void *func_0201ee6c(void *scene, void *table);
extern int func_02023be8(void *object);
extern void func_02023360(void *a, void *b, void *c);
extern void func_020237b4(void *a, void *b, void *c);

void func_02023b28(void)
{
    int mode;
    if (data_020c9e04.next != 0) {
        switch (data_020c9e04.next) {
        case 1:
            mode = 0;
            break;
        default:
            mode = 1;
            break;
        }
        func_02023b9c(mode);
        data_020c9e04.current = data_020c9e04.next;
        data_020c9e04.next = 0;
    }
    if (data_020c9e04.pending != 0) {
        func_02023be8(func_0201ee6c(data_020c9e04.pending, data_020c9e2c));
        data_020c9e04.active = data_020c9e04.pending;
        data_020c9e04.pending = 0;
    }
    func_02023360(data_020c9e48, data_020c9ef0, data_020c9e05);
    func_020237b4(data_020c9e9c, data_020c9f44, data_020c9e05 - 1);
}
