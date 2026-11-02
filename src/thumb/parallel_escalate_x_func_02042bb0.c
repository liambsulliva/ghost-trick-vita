struct Part {
    unsigned char pad00[0x14];
    void *view;
};

extern void *func_0200b208(void);
extern struct Part *func_02023ebc(void *manager, unsigned int key);
extern int func_02042b30(void *view);
extern void func_0200cb60(void *object);
extern int func_02042170(void *object);
extern void *func_02042d18(void *view);
extern void func_020427b0(void *part, void *data, int a, int b, int c, int d, int e, int f, int g, int h, int i);
extern void func_02042d1c(void *view, void *data);
extern void func_02023ef4(void *manager);
extern struct Part *func_02023d60(int size);
extern struct Part *func_02042b50(struct Part *object);
extern void *func_0200883c(void);
extern void *func_0201a4e8(void *object);
extern void *func_0201e7f8(void *loader, int index, unsigned int id, void *dst);
extern void *func_0201e888(void *object, int a, unsigned int b, int c, int d);
extern int func_0200cb5c(void *object);
extern int func_02023f34(void *manager, struct Part *holder, void *file, int value, unsigned int key);

int func_02042bb0(unsigned int id, unsigned char *state, int param_3, int param_4, int param_5, int param_6, int param_7, void *dst)
{
    unsigned int key;
    struct Part *part;
    void *data;
    int size;

    key = (id & 0xffff) << 10 | 0x80000000;
    part = func_02023ebc(func_0200b208(), key);
    if (part != 0) {
        if (func_02042b30(part->view) != 0 && *state != 1) {
            func_0200cb60(part);
            *state = 0xff;
            return func_02042170(part);
        }
        if (*state == 0xff) {
            *state = 0;
        } else if (*state == 1) {
            func_020427b0(part->view, func_02042d18(part->view), 0, param_3, param_4, param_5, id, param_6, param_7, -1, 0x100);
            if (func_02042b30(part->view) != 0) {
                *state = 0xff;
            }
        }
        return func_02042170(part);
    }
    func_02023ef4(func_0200b208());
    part = func_02023d60(0x18);
    if (part != 0) {
        part = func_02042b50(part);
    }
    if (part != 0) {
        if (dst != 0) {
            data = func_0201e7f8(func_0201a4e8(func_0200883c()), 0, id, dst);
        } else {
            data = func_0201e888(func_0201a4e8(func_0200883c()), 0, id, 0, 0);
        }
        size = func_0200cb5c(func_0201a4e8(func_0200883c()));
        if (data != 0) {
            func_020427b0(part->view, data, 0, param_3, param_4, param_5, id, param_6, param_7, -1, 0x100);
            func_02042d1c(part->view, data);
            if (func_02042b30(part->view) != 0) {
                *state = 0xff;
            } else {
                *state = 1;
            }
            if (dst != 0) {
                return func_02023f34(func_0200b208(), part, 0, size, key);
            }
            return func_02023f34(func_0200b208(), part, data, size, key);
        }
    }
    return 0;
}
