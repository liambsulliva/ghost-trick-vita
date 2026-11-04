typedef int code();

void func_02029da4(int param_1, int param_2)
{
  code *pcVar1;
  int value;
  if (param_2 < 0) {
    param_2 = -param_2;
  }
  value = (int)((((unsigned int)(param_2 + 0x14)) + 0x3ff) & 0xfffffc00u) - 0x14;
  pcVar1 = *(code **)(**(int **)(param_1 + 0x10) + 0xc);
  (*pcVar1)(*(int **)(param_1 + 0x10), value);
}
