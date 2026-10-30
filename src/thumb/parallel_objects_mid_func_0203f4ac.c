struct Block16 { int w0; int w1; int w2; int w3; };
extern struct Block16 data_0209af4c;
extern char data_020c9ec8;
extern int func_0203f4f0();
extern int func_0203f1b4();
extern int func_0206f588();
int func_0203f4ac(int *param_1) {
  struct Block16 local;
  int a;
  int b;
  local = data_0209af4c;
  func_0203f4f0(&local);
  a = func_0203f1b4(param_1[9]);
  b = func_0203f1b4(param_1[10]);
  return func_0206f588(&data_020c9ec8, &local, 0x80, 0x60, a, b);
}
