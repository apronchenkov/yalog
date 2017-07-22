#pragma once

#if __APPLE__
#include <os/lock.h>
typedef os_unfair_lock YalogSpinlock;

#define YALOG_SPINLOCK_INIT OS_UNFAIR_LOCK_INIT

static inline void YalogSpinInit(YalogSpinlock *spinlock) {
  *spinlock = OS_UNFAIR_LOCK_INIT;
}

static inline void YalogSpinLock(YalogSpinlock *spinlock) {
  os_unfair_lock_lock(spinlock);
}

static inline void YalogSpinUnlock(YalogSpinlock *spinlock) {
  os_unfair_lock_unlock(spinlock);
}

#else

#include <sched.h>
#include <stdatomic.h>
#include <time.h>

#if defined(_MSC_VER) && (defined(_M_IX86) || defined(_M_X64))
#include <intrin.h>
#define YALOG_SPINLOCK_ASM_VOLATILE_PAUSE() ::_mm_pause()
#elif defined(__i386__) || defined(__x86_64__) || defined(_M_X64)
#define YALOG_SPINLOCK_ASM_VOLATILE_PAUSE() asm volatile("pause")
#elif defined(__arm__) || defined(__aarch64__)
#define YALOG_SPINLOCK_ASM_VOLATILE_PAUSE() asm volatile("yield")
#endif

static inline unsigned int YalogSpinYieldK(unsigned int k) {
  if (k < 4) {
    return k + 1;
  } else if (k < 16) {
    YALOG_SPINLOCK_ASM_VOLATILE_PAUSE();
    return k + 1;
  } else if (k < 32) {
    if (k & 1) {
      sched_yield();
    } else {
      struct timespec ts = {.tv_sec = 0, .tv_nsec = 1000};
      nanosleep(&ts, 0);
    }
    return k + 1;
  } else {
    struct timespec ts = {.tv_sec = 0, .tv_nsec = 500000};
    nanosleep(&ts, 0);
    return k;
  }
}

typedef atomic_flag YalogSpinlock;

#define YALOG_SPINLOCK_INIT ATOMIC_FLAG_INIT

static inline void YalogSpinInit(YalogSpinlock *spinlock) {
  atomic_flag_clear(spinlock);
}

static inline void YalogSpinLock(YalogSpinlock *spinlock) {
  unsigned int k = 0;
  while (atomic_flag_test_and_set_explicit(spinlock, memory_order_acquire)) {
    k = YalogSpinYieldK(k);
  }
}

static inline void YalogSpinUnlock(YalogSpinlock *spinlock) {
  atomic_flag_clear_explicit(spinlock, memory_order_release);
}

#endif
