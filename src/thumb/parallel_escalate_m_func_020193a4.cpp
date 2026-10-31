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
  virtual void s19();
  virtual void s20();
  virtual void s21();
  virtual void s22();
  virtual void s23();
  virtual void s24();
  virtual void s25();
  virtual void s26();
  virtual void s27();
  virtual void s28();
  virtual void s29();
  virtual void s30();
  virtual void s31();
  virtual void s32();
  virtual void s33();
  virtual void s34();
  virtual void s35();
  virtual void s36();
  virtual void s37();
  virtual void s38();
  virtual void s39();
  virtual void s40();
  virtual void s41();
  virtual void s42();
  virtual void s43();
  virtual void s44();
  virtual void s45();
  virtual void s46();
  virtual void s47();
  virtual void s48();
  virtual void s49();
  virtual void s50();
  virtual void s51();
  virtual void s52();
  virtual void s53();
  virtual void s54();
  virtual void s55();
  virtual void s56();
  virtual void s57();
  virtual void s58();
  virtual void s59();
  virtual void s60();
  virtual void s61();
  virtual void s62();
  virtual void s63();
  virtual void s64();
  virtual void s65();
  virtual void s66();
  virtual void s67();
  virtual void s68();
  virtual void s69();
  virtual void s70();
  virtual void s71();
  virtual void s72();
  virtual void s73();
  virtual void s74();
  virtual void s75();
  virtual void s76();
  virtual void s77();
  virtual void s78();
  virtual void s79();
  virtual void s80();
  virtual void s81();
  virtual void s82();
  virtual void s83();
  virtual void s84();
  virtual void s85();
  virtual void s86();
  virtual void s87();
  virtual void s88();
  virtual void s89();
  virtual void s90();
  virtual void s91();
  virtual void s92();
  virtual void s93();
  virtual unsigned short s94(int a, int b);
  unsigned char pad4[0x10 - 4];
  void *model;
  int f14;
  int f18;
  unsigned char pad1c[0x20 - 0x1c];
  unsigned short row;
  unsigned char pad22[0x2a - 0x22];
  unsigned short h2a;
  unsigned char pad2c[0x38 - 0x2c];
  unsigned short kind;
  unsigned char pad3a[0x40 - 0x3a];
  short h40;
  unsigned short h42;
  short h44;
  unsigned short h46;
  unsigned short h48;
  unsigned short h4a;
  unsigned short h4c;
  unsigned char pad4e[0x60 - 0x4e];
  int shadow;
  unsigned char pad64[0x68 - 0x64];
  int list;
  int f6c;
  int f70;
  int f74;
};

struct Found {
  int f0;
  int f4;
  int f8;
};

extern "C" {
extern char data_020b9df4[];
void func_0201778c(Owner *a);
void func_02017714(Owner *a, int b);
Found *func_0201d8ec(int list, unsigned int key);
short func_02017c70(Owner *a, unsigned char kind);
void func_0201c6c4(char *a, void *model);
void func_0201c634(char *a, int b, unsigned short c, unsigned short d, int e);
void func_02016fc4(char *a, unsigned short b);
void func_020194bc(char *a, unsigned short b);
void func_0201cf2c(char *a, int shadow);
void func_02016fbc(char *a, int b);
void func_02016e28(Owner *a);
}

extern "C" void func_020193a4(Owner *param_1, int param_2, int param_3, unsigned int param_4, unsigned char param_5)
{
  Found *f;
  int s;

  if (param_1->list == 0) {
    return;
  }
  param_1->row = 5;
  func_0201778c(param_1);
  func_02017714(param_1, 0);
  param_1->h2a = param_5;
  f = func_0201d8ec(param_1->list, param_4);
  if (f != 0) {
    if (f->f8 == 0) {
      return;
    }
    param_1->f18 = f->f8;
    param_1->f14 = f->f8;
  }
  param_1->h42 = 4;
  param_1->h44 = 0x10;
  param_1->kind = param_1->s94(param_1->f14, 1);
  param_1->h40 = func_02017c70(param_1, param_1->kind);
  if (param_1->kind > 2) {
    s = param_1->h44 - 7;
  }
  else {
    s = param_1->h44 - 4;
  }
  param_1->h44 = s;
  param_1->h46 = 0x100;
  param_1->h48 = 0x40;
  param_1->h4a = param_1->h46;
  param_1->f6c = param_2;
  param_1->h4c = param_1->h46 - 2;
  param_1->f74 = 0;
  param_1->f70 = param_3;
  func_0201c6c4(data_020b9df4, param_1->model);
  func_0201c634(data_020b9df4, param_1->f6c, param_1->h46, param_1->h48, 2);
  func_02016fc4(data_020b9df4, param_1->h46);
  func_020194bc(data_020b9df4, param_1->h48 - 2);
  func_0201cf2c(data_020b9df4, param_1->shadow);
  func_02016fbc(data_020b9df4, 3);
  func_02016e28(param_1);
}
