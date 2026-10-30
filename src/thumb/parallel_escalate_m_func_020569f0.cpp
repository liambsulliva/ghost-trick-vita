struct Flags { unsigned a:1; unsigned b:1; unsigned c:1; unsigned d:1; unsigned e:1; unsigned rest:27; };
extern "C" {
extern void func_02056978(int *);

int *func_020569f0(int *param_1)
{
    func_02056978(param_1);
    *param_1 = (int)0x020adc04u;
    *(int *)((int)param_1 + 0x48) = 0;
    *(int *)((int)param_1 + 0x4c) = 0;
    *(int *)((int)param_1 + 0x50) = -1;
    *(int *)((int)param_1 + 0x54) = -1;
    *(int *)((int)param_1 + 0x58) = -1;
    *(unsigned short *)((int)param_1 + 0x5c) = 0;
    *(unsigned char *)((int)param_1 + 0x5e) = 0xff;
    *(unsigned char *)((int)param_1 + 0x5f) = 1;
    *(unsigned char *)((int)param_1 + 0x60) = 0;
    *(unsigned char *)((int)param_1 + 0x61) = 0;
    *(unsigned char *)((int)param_1 + 0x62) = 0;
    *(int *)((int)param_1 + 0x64) = 0;
    *(int *)((int)param_1 + 0x68) = -1;
    *(int *)((int)param_1 + 0x6c) = 0;
    *(int *)((int)param_1 + 0x70) = 0;
    *(int *)((int)param_1 + 0x74) = 0;
    ((Flags *)((int)param_1 + 0x78))->a = 0;
    ((Flags *)((int)param_1 + 0x78))->b = 0;
    ((Flags *)((int)param_1 + 0x78))->c = 0;
    ((Flags *)((int)param_1 + 0x78))->d = 0;
    ((Flags *)((int)param_1 + 0x78))->e = 1;
    return param_1;
}

}
