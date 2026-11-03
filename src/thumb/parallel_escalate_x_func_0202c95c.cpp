struct Rec {
    unsigned char solid;
    unsigned char lock_off;
    unsigned char need_on;
    unsigned char lock_on;
    unsigned short id;
    unsigned short info;
    char area[0x18];
    char shape[0x10];
};

class Heap {
public:
    virtual void s0();
    virtual void s1();
    virtual void *alloc(unsigned int size, void *tag);
};

class Child {
public:
    virtual void s0();
    virtual void s1();
    virtual void s2();
    virtual void s3();
    virtual void s4();
    virtual void s5();
    virtual void s6();
    virtual void s7();
    virtual void s8();
    virtual void s9();
    virtual void s10();
    virtual void s11();
    virtual void s12();
    virtual void s13();
    virtual void s14();
    virtual void s15();
    virtual int hit(int x, int y);
    char pad4[6];
    unsigned short info;
    char padc[0xc];
    char shape[0x10];
    char area[0x10];
};

class Owner;

class Node {
public:
    virtual void s0();
    virtual void s1();
    virtual void s2();
    virtual void s3();
    virtual void s4();
    virtual void s5();
    virtual void s6();
    virtual void s7();
    virtual void s8();
    virtual void s9();
    virtual void s10();
    virtual void s11();
    virtual void update(Owner *owner, int p2, int p3, unsigned char p4, unsigned char p5);
    char pad4[0x74];
    Node *next;
};

class Drawer {
public:
    virtual void s0();
    virtual void s1();
    virtual void s2();
    virtual void s3();
    virtual void s4();
    virtual void s5();
    virtual void s6();
    virtual void s7();
    virtual void s8();
    virtual void s9();
    virtual void s10();
    virtual void s11();
    virtual void s12();
    virtual void s13();
    virtual void s14();
    virtual void s15();
    virtual void s16();
    virtual void s17();
    virtual void s18();
    virtual void draw(void *data, int a, int b, int c, int d);
};

class Owner {
public:
    virtual void s0();
    virtual void s1();
    virtual void s2();
    virtual void s3();
    virtual void s4();
    virtual void s5();
    virtual void s6();
    virtual void s7();
    virtual void s8();
    virtual Child *create(unsigned short id);
    char pad4[0x6c];
    Drawer *drawer;
    Node *nodes;
    char pad78[0xc];
    unsigned int count;
    int *offsets;
    char *data;
    char pad90[0x10];
    int *base;
    char *table;
    unsigned char ready;
    char pada9[3];
    Child **children;
};

extern "C" {
extern char data_02085298[];
extern void *func_02007bd8(void);
extern Heap *func_02007b70(void *heap);
extern void *func_02010b7c(void);
extern int func_02010b60(void *touch);
extern int func_02010b64(void *touch);
extern int func_02015154(void *touch);
extern int func_02015188(void *touch);
extern int func_0202cb64(void *touch);
extern void func_0202b284(void *dst, void *src);
extern void func_0202bcf0(void *dst, void *src);
extern void func_0202b4fc(Child *child, Owner *owner);
extern void func_0202cb48(Child *child);
extern void func_0202cb58(Child *child);
int func_0202c95c(Owner *owner, int p1, int p2, int p3, unsigned char p4, unsigned char p5);
}

int func_0202c95c(Owner *owner, int p1, int p2, int p3, unsigned char p4, unsigned char p5)
{
    unsigned short i;
    Rec *rec;
    Child *child;
    int flag;
    int sel;
    int found;
    Node *node;
    Node *cur;

    if (owner->ready == 0) {
        owner->children = (Child **)func_02007b70(func_02007bd8())->alloc(owner->count * 4, data_02085298);
        for (i = 0; i < owner->count; i++) {
            rec = (Rec *)(owner->data + owner->offsets[i]);
            child = owner->create(rec->id);
            if (child != 0) {
                func_0202b284(child->area, &rec->area[0]);
                child->info = rec->info;
                func_0202bcf0(child->shape, rec->shape);
                func_0202b4fc(child, owner);
                owner->children[i] = child;
            }
        }
        owner->ready = 1;
    }
    flag = 0;
    if (func_02015154(func_02010b7c()) != 0) {
        flag = 1;
    }
    sel = 0;
    found = 0;
    for (i = 0; i < owner->count; i++) {
        rec = (Rec *)(owner->data + owner->offsets[i]);
        if (rec->solid != 0 && rec->lock_on != 0) {
            if (found == 0) {
                sel = i;
            }
            continue;
        }
        if (rec->lock_off != 0 && flag == 0) {
            continue;
        }
        if (rec->lock_on != 0 && flag != 0) {
            continue;
        }
        if (rec->need_on != 0 && (flag == 0 || found != 0)) {
            continue;
        }
        if (owner->children[i]->hit(func_02010b60(func_02010b7c()), func_02010b64(func_02010b7c())) != 0) {
            found = 1;
            sel = i;
        }
    }
    for (i = 0; i < owner->count; i++) {
        if (sel == i) {
            func_0202cb48(owner->children[i]);
        } else {
            func_0202cb58(owner->children[i]);
        }
    }
    if (func_02015188(func_02010b7c()) != 0 && func_0202cb64(func_02010b7c()) == 0) {
        owner->drawer->draw(owner->table + *owner->base + 0xc, 0, 0, 0, 0);
    }
    node = owner->nodes;
    while (node != 0) {
        cur = node;
        node = node->next;
        cur->update(owner, p2, p3, p4, p5);
    }
    return 1;
}
