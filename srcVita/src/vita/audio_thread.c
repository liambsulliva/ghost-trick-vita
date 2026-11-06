#include <stddef.h>
#include <psp2/kernel/cpu.h>
#include <psp2/kernel/threadmgr.h>
#include <psp2/power.h>
#include "audio_thread.h"

#define THREAD_STACK_SIZE (64 * 1024)
#define THREAD_PRIORITY 64

static audio_thread_step steps[AUDIO_THREAD_KIND_COUNT];
static SceUID threads[AUDIO_THREAD_KIND_COUNT];
static SceUID mailbox_lock = -1;

static int audio_thread_body(SceSize argument_size, void *arguments)
{
    enum audio_thread_kind kind;

    (void)argument_size;
    kind = *(enum audio_thread_kind *)arguments;
    while (steps[kind]()) {
    }
    return 0;
}

int audio_thread_start(enum audio_thread_kind kind, audio_thread_step step)
{
    static const char *const names[AUDIO_THREAD_KIND_COUNT] = { "music_worker", "sound_worker" };
    static const int cpu_masks[AUDIO_THREAD_KIND_COUNT] = { SCE_KERNEL_CPU_MASK_USER_2, SCE_KERNEL_CPU_MASK_USER_1 };
    SceUID thread;
    int result;

    if (mailbox_lock < 0) {
        mailbox_lock = sceKernelCreateMutex("audio_mailbox", 0, 0, NULL);
        if (mailbox_lock < 0) {
            return mailbox_lock;
        }
    }
    steps[kind] = step;
    thread = sceKernelCreateThread(names[kind], audio_thread_body, THREAD_PRIORITY, THREAD_STACK_SIZE, 0, cpu_masks[kind], NULL);
    if (thread < 0) {
        return thread;
    }
    threads[kind] = thread;
    result = sceKernelStartThread(thread, sizeof(kind), &kind);
    if (result < 0) {
        sceKernelDeleteThread(thread);
        return result;
    }
    return 0;
}

void audio_thread_wait_for_end(enum audio_thread_kind kind, unsigned int limit_us)
{
    SceUInt timeout;

    timeout = limit_us;
    if (sceKernelWaitThreadEnd(threads[kind], NULL, &timeout) >= 0) {
        sceKernelDeleteThread(threads[kind]);
    }
}

void audio_thread_sleep_us(unsigned int duration_us)
{
    sceKernelDelayThread(duration_us);
}

void audio_thread_read_facts(struct audio_thread_facts *facts)
{
    unsigned int fpscr;

    __asm__ volatile("vmrs %0, fpscr" : "=r"(fpscr));
    facts->priority = sceKernelGetThreadCurrentPriority();
    facts->cpu_mask = (unsigned int)sceKernelGetThreadCpuAffinityMask(sceKernelGetThreadId());
    facts->cpu = sceKernelGetCpuId();
    facts->arm_mhz = scePowerGetArmClockFrequency();
    facts->fpscr = fpscr;
}

void audio_mailbox_lock(void)
{
    sceKernelLockMutex(mailbox_lock, 1, NULL);
}

void audio_mailbox_unlock(void)
{
    sceKernelUnlockMutex(mailbox_lock, 1);
}
