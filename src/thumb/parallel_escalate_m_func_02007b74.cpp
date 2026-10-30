class Child {
public:
    virtual ~Child();
};
extern "C" {
struct Manager {
    void (**table)(struct Manager *manager,int handle);
};
struct Object {
    void *table;
    int words[4];
    char *owner;
    int handle;
    Child *child;
    int word20;
    int entry;
};
extern char data_020a2b14;
extern void *func_02007bd8(void);
extern struct Manager *func_02007b70(void *system);
extern void func_0200bab8(char *list,int entry);
extern void func_02007ab4(struct Object *object);
extern void func_02031270(void *object);
struct Object *func_02007b74(struct Object *object)
{
    struct Manager *manager;
    object->table = &data_020a2b14;
    if (object->handle != 0) {
        if (func_02007b70(func_02007bd8()) != 0) {
            manager = func_02007b70(func_02007bd8());
            manager->table[4](manager,object->handle);
            object->handle = 0;
        }
    }
    if (object->child != 0) {
        delete object->child;
        object->child = 0;
    }
    if (object->entry != 0) {
        func_0200bab8(object->owner + 0x28,object->entry);
    }
    func_02007ab4(object);
    func_02031270(object);
    return object;
}
}
