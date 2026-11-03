class Base020d3eb8 {
public:
    Base020d3eb8() { f04 = 0; f08 = 0; }
    virtual ~Base020d3eb8();
    virtual int m2() = 0;
    virtual int m3() = 0;
    virtual int m4() = 0;
    virtual int m5() = 0;
    virtual int m6() = 0;
    int f04;
    int f08;
};
class File020d5134 : public Base020d3eb8 {
public:
    virtual ~File020d5134();
    virtual int m2();
    virtual int m3();
    virtual int m4();
    virtual int m5();
    virtual int m6();
    int Open(int name);
    void *operator new(unsigned long size);
    int f0c;
    int f10;
};
class Obj020d3ee8 {
public:
    Obj020d3ee8();
    ~Obj020d3ee8();
    int Init(File020d5134 *file, int mode);
    void *operator new(unsigned long size);
    void operator delete(void *p);
    int w[54];
};
extern "C" Obj020d3ee8 *func_ov004_020d3bdc(int name, int mode)
{
    File020d5134 *file;
    Obj020d3ee8 *obj;
    file = new File020d5134;
    if (file == 0) {
        return 0;
    }
    if (file->Open(name) == 0) {
        delete file;
        return 0;
    }
    obj = new Obj020d3ee8;
    if (obj->Init(file, mode) == 0) {
        delete obj;
        return 0;
    }
    return obj;
}
