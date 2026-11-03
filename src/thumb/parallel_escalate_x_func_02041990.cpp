class Base02022d70 {
public:
    Base02022d70();
    virtual ~Base02022d70();
    unsigned int b04[7];
};

class Obj02041990 : public Base02022d70 {
public:
    Obj02041990();
    virtual ~Obj02041990();
    unsigned int f20[5];
    unsigned int f34;
    unsigned int f38;
    unsigned int f3c;
    unsigned int f40;
    unsigned char f44;
    unsigned char f45;
    unsigned short f46;
    unsigned int f48;
    unsigned short f4c;
    unsigned char f4e;
    unsigned char f4f;
    unsigned char f50;
    unsigned char b0 : 1;
    unsigned char b1 : 1;
    unsigned char b2 : 1;
    unsigned char b3 : 5;
};

Obj02041990::Obj02041990() {
    signed char i;
    f34 = 0;
    f38 = 0;
    f3c = 0;
    f40 = 0;
    f44 = 0x78;
    f45 = 4;
    f48 = 0;
    f4c = 0;
    f4e = 0;
    b0 = 0;
    b1 = 0;
    b2 = 0;
    for (i = 0; i < 5; i++) {
        f20[i] = 0;
    }
}
