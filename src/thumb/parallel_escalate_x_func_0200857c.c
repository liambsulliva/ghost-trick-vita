typedef struct File0857c File0857c;

typedef struct FileVt0857c {
    void *s[6];
    void (*seek)(File0857c *f, unsigned int pos, int whence);
    void (*read)(File0857c *f, void *buf, unsigned int size);
} FileVt0857c;

struct File0857c {
    FileVt0857c *vt;
};

typedef struct Heap0857c Heap0857c;

typedef struct HeapVt0857c {
    void *s[3];
    void *(*alloc)(Heap0857c *h, unsigned int size);
} HeapVt0857c;

struct Heap0857c {
    HeapVt0857c *vt;
};

typedef struct Entry0857c {
    unsigned int pos;
    unsigned int size;
} Entry0857c;

typedef struct Owner0857c {
    char pad0[0x28];
    int vram[0xa];
    unsigned short index;
} Owner0857c;

typedef struct Tex0857c Tex0857c;

struct Tex0857c {
    char pad0[0x12];
    unsigned char dirty : 1;
    unsigned char loaded : 1;
    unsigned char rest : 6;
    char pad13;
    Owner0857c *owner;
    Entry0857c *entries;
    File0857c *file;
    unsigned int cap;
    int handle;
    Tex0857c *parent;
};

extern void func_0200bad8(int *vram, void *src, unsigned int size, int handle);
extern void *func_02007bd8(void);
extern void *func_020086ac(void *a);
extern unsigned int func_020086c0(void *buf);
extern void func_0200bab8(int *vram, int handle);
extern int func_0200ba98(int *vram, unsigned int size);
extern Heap0857c *func_02007b70(void *a);
extern void func_02074860(void *buf, unsigned int size);
extern void func_02073174(void *src, void *dst);

void func_0200857c(Tex0857c *t)
{
    unsigned int size;
    void *buf;
    void *dst;
    Heap0857c *heap;

    if (!t->dirty || t->loaded) {
        return;
    }
    if (t->parent != 0) {
        if (!t->parent->loaded) {
            t->parent->dirty = 1;
            func_0200857c(t->parent);
            t->parent->dirty = 0;
        }
        t->dirty = t->parent->dirty;
        t->loaded = t->parent->loaded;
        func_0200bad8(t->owner->vram, 0, 0, t->parent->handle);
        return;
    }
    size = t->entries[t->owner->index].size;
    t->file->vt->seek(t->file, t->entries[t->owner->index].pos, 0);
    buf = func_020086ac(func_02007bd8());
    t->file->vt->read(t->file, buf, size);
    size = func_020086c0(buf);
    if (t->cap < size) {
        if (t->handle != 0) {
            func_0200bab8(t->owner->vram, t->handle);
            t->cap = 0;
        }
        t->handle = func_0200ba98(t->owner->vram, size);
    }
    if (t->handle != -1) {
        if (t->cap < size) {
            t->cap = size;
        }
        heap = func_02007b70(func_02007bd8());
        dst = heap->vt->alloc(heap, size);
        if (dst != 0) {
            func_02074860(buf, size);
            func_02073174(buf, dst);
            func_0200bad8(t->owner->vram, dst, size, t->handle);
            t->dirty = 0;
            t->loaded = 1;
        }
    } else {
        t->cap = 0;
    }
}
