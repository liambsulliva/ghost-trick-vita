extern void func_0202c038(int object);
extern void func_0202d700(int object,int key,unsigned int value);
extern void func_0202d710(int object,int key,unsigned int value);

struct Table {
    unsigned char before[0xe4];
    int keys[8];
    unsigned int first[8];
    unsigned int second[8];
};

void func_0202d6c0(struct Table *object)
{
    unsigned int index;

    func_0202c038((int)object);
    index = 0;
    while (index < 8) {
        if (object->keys[index] >= 0) {
            func_0202d700((int)object,object->keys[index],object->first[index]);
            func_0202d710((int)object,object->keys[index],object->second[index]);
        }
        index = index + 1;
    }
}
