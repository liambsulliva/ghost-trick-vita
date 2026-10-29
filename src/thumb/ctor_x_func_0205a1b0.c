struct Object {
    void *methods;
};

extern char data_020ae904;
void func_0206338c(void *object);
void func_0202b164(void *object);
void func_0200f70c(void *object);

struct Object *func_0205a1b0(struct Object *object)
{
    func_0206338c(object);
    object->methods = &data_020ae904;
    func_0202b164((char *)object + 172);
    func_0202b164((char *)object + 180);
    func_0202b164((char *)object + 188);
    func_0202b164((char *)object + 196);
    func_0200f70c((char *)object + 204);
    func_0200f70c((char *)object + 216);
    func_0200f70c((char *)object + 228);
    func_0200f70c((char *)object + 240);
    *(int *)((char *)object + 16) = 0;
    return object;
}
