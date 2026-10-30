struct Manager {
    void (**table)(struct Manager *manager,int handle);
};
struct Scene {
    void *table;
    char sub[0xee0];
    void *buffer;
    int wordee8;
    char first[4];
    char second[0x14];
    char third[0x144];
    int handles[4];
};
extern char data_020a2f94;
extern void func_02031270(void *object);
extern void *func_02007bd8(void);
extern struct Manager *func_02007b70(void *system);
extern void func_0202f64c(void *object);
extern void func_0200e784(void *object);
extern void func_0200e760(void *object);
extern void func_02007ab4(void *object);

struct Scene *func_0200e790(struct Scene *scene)
{
    unsigned int i;
    struct Manager *manager;

    scene->table = &data_020a2f94;
    if (scene->buffer != 0) {
        func_02031270(scene->buffer);
        scene->buffer = 0;
    }
    for (i = 0; i < 4; i++) {
        if (i != 3 && *(int *)((char *)scene + i * 4 + 0x1048) != 0) {
            if (func_02007b70(func_02007bd8()) != 0) {
                manager = func_02007b70(func_02007bd8());
                manager->table[4](manager,*(int *)((int)scene + i * 4 + 0x1048));
                *(int *)((unsigned int)scene + i * 4 + 0x1048) = 0;
            }
        }
    }
    func_0202f64c(scene->first);
    func_0202f64c(scene->second);
    func_0200e784(scene->third);
    func_0200e760(scene->sub);
    func_02007ab4(scene);
    return scene;
}
